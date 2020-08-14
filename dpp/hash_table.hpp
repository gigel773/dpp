#ifndef DPP_HASH_TABLE_HPP
#define DPP_HASH_TABLE_HPP

#include "defs.hpp"

namespace dpp
{
    constexpr const uint32_t MINIMAL_MATCH_LENGTH = 3;
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
}

#endif //DPP_HASH_TABLE_HPP
