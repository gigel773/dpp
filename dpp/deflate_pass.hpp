#ifndef DPP_DEFLATE_PASS_HPP
#define DPP_DEFLATE_PASS_HPP

#include "defs.hpp"
#include "huffman_coding.hpp"
#include "hash_table.hpp"
#include "histogram.hpp"
#include "unpacking.hpp"
#include "util.hpp"

namespace dpp
{
    namespace internal
    {
        template<class iterator_t,
                class match_handler_t,
                class literal_handler_t>
        void deflate_pass(const iterator_t &src_begin,
                          const iterator_t &src_end,
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
                    hash_table[current_byte] = std::distance(src_begin, current_byte);
                    current_byte += match_length;
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
    auto deflate(const input_iterator_t &src_begin,
                 const input_iterator_t &src_end,
                 const output_iterator_t &dst_begin) -> uint32_t
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
        uint64_t bits_written = 0;

        internal::deflate_pass(src_begin,
                               src_end,
                               [&](histogram_t::match_t match)
                               {
                                   const auto match_code_length  = match_lengths_unpacked[match.length - 3].code_length;
                                   const auto offset_code_length = offsets_unpacked[match.offset].code_length;

                                   if (match_code_length > 16 || offset_code_length > 16)
                                   {
                                       throw std::out_of_range("Code length is bigger than 16");
                                   }

                                   bits_written += match_code_length + offset_code_length;
                               },
                               [&](uint8_t literal)
                               {
                                   const auto code_length = literals_matches_alphabet[literal].code_length;

                                   if (code_length > 16)
                                   {
                                       throw std::out_of_range("Code length is bigger than 16");
                                   }

                                   bits_written += code_length;
                               });

        return (bits_written / 8) + (bits_written % 8 ? 1 : 0);
    }
}

#endif //DPP_DEFLATE_PASS_HPP
