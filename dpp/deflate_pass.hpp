#ifndef DPP_DEFLATE_PASS_HPP
#define DPP_DEFLATE_PASS_HPP

#include <cstdint>
#include <array>
#include <immintrin.h>

#include "huffman_coding.hpp"

namespace dpp
{
    constexpr const uint32_t MINIMAL_MATCH_LENGTH = 4;
    constexpr const uint32_t HISTORY_BUFFER_SIZE  = 32768;

    class hash_table_t
    {
        static constexpr const uint32_t HASH_MASK = HISTORY_BUFFER_SIZE - 1;

    public:
        constexpr hash_table_t()
        {
            for (int32_t i = 0; i < HISTORY_BUFFER_SIZE; i++)
            {
                hash_table_[i] = -1;
            }
        }

        template<class iterator_t>
        auto operator[](iterator_t source) -> int32_t &
        {
            static_assert(std::is_same<typename std::iterator_traits<iterator_t>::iterator_category,
                                  std::random_access_iterator_tag>::value,
                          "Invalid iterator passed (should be random access)");

            const uint32_t hash = HASH_MASK & _mm_crc32_u32(*reinterpret_cast<uint32_t *>(&*source), 0);

            return hash_table_[hash];
        }

    private:
        std::array<int32_t, HISTORY_BUFFER_SIZE> hash_table_{};
    };

    namespace internal
    {
        template<class iterator_t>
        auto compare(iterator_t left, iterator_t right) -> int32_t
        {
            const iterator_t boundary     = right;
            int32_t          match_length = 0;

            while (left < boundary &&
                   *left == *right)
            {
                match_length++;
                left++;
                right++;
            }

            return match_length >= MINIMAL_MATCH_LENGTH ? match_length : -1;
        }

        auto get_length_index(int32_t length) -> uint32_t;

        auto get_offset_index(int32_t offset) -> uint32_t;
    }

    template<class input_iterator_t,
            class output_iterator_t>
    void deflate(const input_iterator_t &src_begin,
                 const input_iterator_t &src_end,
                 const output_iterator_t &dst_begin,
                 const output_iterator_t &dst_end)
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
                const int32_t match_length = internal::compare(candidate, current_byte);
                const int32_t offset       = std::distance(candidate, current_byte);

                if (match_length == -1 || offset > HISTORY_BUFFER_SIZE)
                {
                    literals_matches_histogram[*current_byte]++;
                    hash_table[current_byte] = std::distance(src_begin, current_byte);
                    current_byte++;
                    continue;
                }

                literals_matches_histogram[internal::get_length_index(match_length)]++;
                offsets_histogram[internal::get_offset_index(offset)]++;

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
