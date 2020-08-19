#ifndef DPP_DEFS_HPP
#define DPP_DEFS_HPP

#include <cstdint>
#include <array>
#include <immintrin.h>
#include <type_traits>
#include <iostream>
#include <vector>
#include <algorithm>

namespace dpp
{
    constexpr const uint32_t MAX_CODE_LENGTH                   = 16;
    constexpr const uint32_t RLE_MAX_CODE_LENGTH               = 7;
    constexpr const uint32_t LITERALS_MATCH_LENGTHS_TABLE_SIZE = 286;
    constexpr const uint32_t OFFSETS_TABLE_SIZE                = 30;
    constexpr const uint32_t MATCH_LENGTHS_TABLE_SIZE          = 255;
    constexpr const uint32_t RLE_ALPHABET                      = 19;

    constexpr auto get_max_code_length(uint32_t table_size) -> uint32_t
    {
        if (table_size == RLE_ALPHABET)
        {
            return RLE_MAX_CODE_LENGTH;
        } else
        {
            return MAX_CODE_LENGTH;
        }
    }

    namespace huff
    {
        struct code
        {
            uint32_t code;
            uint8_t  code_length;

            bool operator==(const huff::code &rhs) const
            {
                return code_length == rhs.code_length;
            }

            bool operator!=(const huff::code &rhs) const
            {
                return !(rhs == *this);
            }
        };
    }
}
#endif //DPP_DEFS_HPP
