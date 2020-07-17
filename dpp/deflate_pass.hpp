#ifndef DPP_DEFLATE_PASS_HPP
#define DPP_DEFLATE_PASS_HPP

#include "defs.hpp"
#include "huffman_coding.hpp"
#include "hash_table.hpp"
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
        auto                     current_byte = src_begin;
        hash_table_t             hash_table{};
        std::array<int16_t, 286> literals_matches_histogram{};
        std::array<int16_t, 30>  offsets_histogram{};

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
                    literals_matches_histogram[*current_byte]++;
                    hash_table[current_byte] = std::distance(src_begin, current_byte);
                    current_byte++;
                    continue;
                }

                literals_matches_histogram[util::get_length_index(match_length)]++;
                offsets_histogram[util::get_offset_index(offset)]++;

                hash_table[current_byte] = std::distance(src_begin, current_byte);

                current_byte += match_length;
            } else
            {
                literals_matches_histogram[*current_byte]++;
                hash_table[current_byte] = std::distance(src_begin, current_byte);
                current_byte++;
            }
        }

        for (auto symbol = current_byte; symbol < src_end; symbol++)
        {
            literals_matches_histogram[*symbol]++;
        }

        // Build tables
        std::array<huff::code, literals_matches_histogram.size()> literals_matches_alphabet{};
        std::array<huff::code, offsets_histogram.size()>          offsets_alphabet{};

        huff::build_huffman_alphabet<huff::LITERALS_MATCH_LENGTHS_TABLE_SIZE>(literals_matches_histogram,
                                                                              literals_matches_alphabet);
        huff::build_huffman_alphabet<huff::OFFSETS_TABLE_SIZE>(offsets_histogram, offsets_alphabet);
    }
}

#endif //DPP_DEFLATE_PASS_HPP
