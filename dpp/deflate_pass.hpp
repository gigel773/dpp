#ifndef DPP_DEFLATE_PASS_HPP
#define DPP_DEFLATE_PASS_HPP

#include "defs.hpp"
#include "bit_manipulation.hpp"
#include "huffman_coding.hpp"
#include "hash_table.hpp"
#include "histogram.hpp"
#include "unpacking.hpp"
#include "rle_coding.hpp"
#include "bit_stream.hpp"
#include "util.hpp"

namespace dpp
{
    namespace internal
    {
        template<class iterator_t,
                class match_handler_t,
                class literal_handler_t>
        void deflate_pass(const iterator_t src_begin,
                          const iterator_t src_end,
                          match_handler_t match_handler,
                          literal_handler_t literal_handler)
        {
            hash_table_t hash_table{};

            auto current_byte = src_begin;

            while (current_byte < src_end - MINIMAL_MATCH_LENGTH)
            {
                const uint32_t candidate_idx = hash_table[current_byte];
                const auto     candidate     = src_begin + candidate_idx;

                if (candidate_idx != -1)
                {
                    const int32_t match_length = util::compare(candidate, current_byte);
                    const int32_t offset       = std::distance(candidate, current_byte);

                    if (match_length == -1 || offset > HISTORY_BUFFER_SIZE)
                    {
                        literal_handler(*current_byte);

                        hash_table[current_byte] = std::distance(src_begin, current_byte);
                        current_byte++;

                        continue;
                    }

                    match_handler({match_length, offset});
                    const auto upper_boundary = current_byte + match_length;

                    while (current_byte < upper_boundary)
                    {
                        hash_table[current_byte] = std::distance(src_begin, current_byte);
                        current_byte++;
                    }
                } else
                {
                    literal_handler(*current_byte);

                    hash_table[current_byte] = std::distance(src_begin, current_byte);
                    current_byte++;
                }
            }

            for (auto symbol = current_byte; symbol < src_end; symbol++)
            {
                literal_handler(*symbol);
            }
        }
    }

    template<class input_iterator_t,
            class output_iterator_t>
    auto deflate(const input_iterator_t src_begin,
                 const input_iterator_t src_end,
                 const output_iterator_t dst_begin) -> uint32_t
    {
        // Variables
        histogram_t histogram{};

        // Build histogram
        internal::deflate_pass(src_begin,
                               src_end,
                               [&](histogram_t::match_t match)
                               {
                                   histogram.update(match);
                               },
                               [&](uint8_t literal)
                               {
                                   histogram.update(literal);
                               });

        // Build tables
        std::array<huff::code, LITERALS_MATCH_LENGTHS_TABLE_SIZE> literals_matches_alphabet{};
        std::array<huff::code, OFFSETS_TABLE_SIZE>                offsets_alphabet{};

        huff::build_huffman_alphabet<LITERALS_MATCH_LENGTHS_TABLE_SIZE>(histogram.get_literals_histogram(),
                                                                        literals_matches_alphabet);
        huff::build_huffman_alphabet<OFFSETS_TABLE_SIZE>(histogram.get_offsets_histogram(), offsets_alphabet);

        // Unpack tables
        auto match_lengths_unpacked = unpack_match_lengths_table(literals_matches_alphabet);
        auto offsets_unpacked       = unpack_offsets_table(offsets_alphabet);

        // Encode the text
        uint64_t                                 bits_written = 17;
        std::array<int16_t, RLE_ALPHABET>        rle_codes{};
        std::array<huff::code, RLE_ALPHABET>     rle_alphabet{};
        std::vector<std::pair<uint8_t, uint8_t>> rle_instructions;

        rle_instructions.reserve(literals_matches_alphabet.size() + offsets_alphabet.size());

        auto rle_instruction_handler = [&rle_codes, &rle_instructions](rle::instruction it)
        {
            rle_instructions.emplace_back(it.symbol, 1);

            if (it.count == 0)
            {
                rle_codes[it.symbol]++;
            } else if (it.symbol == 0)
            {
                const size_t grand_repeats   = it.count / 138;
                const size_t grand_remainder = it.count % 138;

                rle_codes[18] += grand_repeats;
                for (size_t i = 0; i < grand_repeats; i++)
                {
                    rle_instructions.emplace_back(18, 138);
                }

                if (grand_remainder >= 11)
                {
                    rle_codes[18]++;
                    rle_instructions.emplace_back(18, grand_remainder);
                } else
                {
                    const size_t repeats   = grand_remainder / 10;
                    const size_t remainder = grand_remainder % 10;

                    rle_codes[17] += repeats;
                    for (size_t i = 0; i < repeats; i++)
                    {
                        rle_instructions.emplace_back(17, 10);
                    }

                    if (remainder >= 3)
                    {
                        rle_codes[17]++;
                        rle_instructions.emplace_back(17, remainder);
                    } else
                    {
                        rle_codes[it.symbol] += remainder;
                        for (size_t i = 0; i < remainder; i++)
                        {
                            rle_instructions.emplace_back(it.symbol, 1);
                        }
                    }
                }
            } else
            {
                const size_t repeats   = it.count / 6;
                const size_t remainder = it.count % 6;

                rle_codes[16] += repeats;

                for (size_t i = 0; i < repeats; i++)
                {
                    rle_instructions.emplace_back(16, 6);
                }

                if (remainder >= 3)
                {
                    rle_codes[16]++;
                    rle_instructions.emplace_back(16, remainder);
                } else
                {
                    rle_codes[it.symbol] += remainder;
                    for (size_t i = 0; i < remainder; i++)
                    {
                        rle_instructions.emplace_back(it.symbol, 1);
                    }
                }
            }
        };

        rle::run(literals_matches_alphabet.begin(),
                 literals_matches_alphabet.end(),
                 rle_instruction_handler,
                 [](huff::code it) -> uint8_t
                 { return it.code_length; });

        rle::run(offsets_alphabet.begin(),
                 offsets_alphabet.end(),
                 rle_instruction_handler,
                 [](huff::code it) -> uint8_t
                 { return it.code_length; });

        huff::build_huffman_alphabet<RLE_ALPHABET>(rle_codes, rle_alphabet);

        // Write header
        bit_stream stream(dst_begin);

        // Block final
        stream.write((uint8_t) 1, 1);

        // Block dynamic
        stream.write((uint8_t) 2, 2);

        // Write HLIT, HDIST, HCLEN
        stream.write((uint8_t) (literals_matches_alphabet.size() - 257), 5);
        stream.write((uint8_t) (offsets_alphabet.size() - 1), 5);
        stream.write((uint8_t) (rle_alphabet.size() - 4), 4);

        constexpr std::array<uint8_t, RLE_ALPHABET> rle_lengths_indices = {
                16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
        };

        for (auto it: rle_lengths_indices)
        {
            stream.write(rle_alphabet[it].code_length, 3);
        }

        for (auto &it: rle_instructions)
        {
            const auto code = rle_alphabet[it.first];
            stream.write(code.code, code.code_length, true);

            switch (it.first)
            {
                case 16:
                    stream.write(static_cast<uint8_t>(it.second - 3), 2);
                    break;
                case 17:
                    stream.write(static_cast<uint8_t>(it.second - 3), 3);
                    break;
                case 18:
                    stream.write(static_cast<uint8_t>(it.second - 11), 7);
                    break;
                default:
                    break;
            }
        }

        internal::deflate_pass(src_begin,
                               src_end,
                               [&](histogram_t::match_t match)
                               {
                                   const auto match_code  = match_lengths_unpacked[match.length - 3];
                                   const auto offset_code = offsets_unpacked[match.offset];

                                   stream.write(match_code.code, match_code.code_length);
                                   stream.write(offset_code.code, offset_code.code_length);
                               },
                               [&](uint8_t literal)
                               {
                                   const auto code = literals_matches_alphabet[literal];

                                   stream.write(code.code, code.code_length, true);
                               });

        // Write EOB
        stream.write(literals_matches_alphabet[256].code, literals_matches_alphabet[256].code_length);
        stream.flush();

        return stream.get_written_bytes();
    }
}

#endif //DPP_DEFLATE_PASS_HPP
