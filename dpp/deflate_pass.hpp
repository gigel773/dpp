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
    template<class input_iterator_t,
            class output_iterator_t>
    void deflate(const input_iterator_t &src_begin,
                 const input_iterator_t &src_end,
                 const output_iterator_t &dst_begin)
    {
        // Variables
        auto         current_byte = src_begin;
        hash_table_t hash_table{};
        histogram_t  histogram{};

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
                    histogram.update(*current_byte);
                    hash_table[current_byte] = std::distance(src_begin, current_byte);
                    current_byte++;

                    continue;
                }

                histogram.update({match_length, offset});
                hash_table[current_byte] = std::distance(src_begin, current_byte);
                current_byte += match_length;
            } else
            {
                histogram.update(*current_byte);
                hash_table[current_byte] = std::distance(src_begin, current_byte);
                current_byte++;
            }
        }

        for (auto symbol = current_byte; symbol < src_end; symbol++)
        {
            histogram.update(*current_byte);
        }

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
    }
}

#endif //DPP_DEFLATE_PASS_HPP
