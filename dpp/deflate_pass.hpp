#ifndef DPP_DEFLATE_PASS_HPP
#define DPP_DEFLATE_PASS_HPP

#include <cstdint>
#include <array>
#include <immintrin.h>

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
            }

            return match_length >= MINIMAL_MATCH_LENGTH ? match_length : -1;
        }

        auto get_length_index(int32_t length) -> uint32_t
        {
            if (length < 11)
            {
                return 257 + length - 3;
            } else if (length < 19)
            {
                return 261 + (length - 3) / 2;
            } else if (length < 35)
            {
                return 265 + (length - 3) / 4;
            } else if (length < 67)
            {
                return 269 + (length - 3) / 8;
            } else if (length < 131)
            {
                return 273 + (length - 3) / 16;
            } else if (length < 258)
            {
                return 277 + (length - 3) / 32;
            } else
            {
                return 285;
            }
        }

        auto get_offset_index(int32_t offset) -> uint32_t
        {
            if (offset <= 2)
            {
                return offset - 1;
            } else if (offset <= 4)
            {
                return 0 + (offset - 1) / 1;
            } else if (offset <= 8)
            {
                return 2 + (offset - 1) / 2;
            } else if (offset <= 16)
            {
                return 4 + (offset - 1) / 4;
            } else if (offset <= 32)
            {
                return 6 + (offset - 1) / 8;
            } else if (offset <= 64)
            {
                return 8 + (offset - 1) / 16;
            } else if (offset <= 128)
            {
                return 10 + (offset - 1) / 32;
            } else if (offset <= 256)
            {
                return 12 + (offset - 1) / 64;
            } else if (offset <= 512)
            {
                return 14 + (offset - 1) / 128;
            } else if (offset <= 1024)
            {
                return 16 + (offset - 1) / 256;
            } else if (offset <= 2048)
            {
                return 18 + (offset - 1) / 512;
            } else if (offset <= 4096)
            {
                return 20 + (offset - 1) / 1024;
            } else if (offset <= 8192)
            {
                return 22 + (offset - 1) / 2048;
            } else if (offset <= 16384)
            {
                return 24 + (offset - 1) / 4096;
            } else if (offset <= 32768)
            {
                return 26 + (offset - 1) / 8192;
            } else
            {
                return ~0u;    /* ~0 is an invalid distance code */
            }
        }
    }

    template<class input_iterator_t,
            class output_iterator_t>
    void deflate(const input_iterator_t &src_begin,
                 const input_iterator_t &src_end,
                 const output_iterator_t &dst_begin,
                 const output_iterator_t &dst_end)
    {
        // Variables
        auto                      current_byte = src_begin;
        hash_table_t              hash_table{};
        std::array<uint32_t, 286> literals_matches_histogram{};
        std::array<uint32_t, 30>  offsets_histogram{};

        while (current_byte < src_end - MINIMAL_MATCH_LENGTH)
        {
            const uint32_t candidate_idx = hash_table[current_byte];
            const auto     candidate     = src_begin + candidate_idx;

            if (candidate_idx != -1)
            {
                const int32_t match_length = internal::compare(candidate, current_byte);
                const int32_t offset       = std::distance(candidate, current_byte);

                if (match_length == -1)
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
    }
}

#endif //DPP_DEFLATE_PASS_HPP
