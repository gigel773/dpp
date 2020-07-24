#ifndef DPP_DEFS_HPP
#define DPP_DEFS_HPP

#include <cstdint>
#include <array>
#include <immintrin.h>
#include <type_traits>
#include <iostream>

namespace dpp
{
    constexpr const uint32_t MAX_CODE_LENGTH                   = 16;
    constexpr const uint32_t LITERALS_MATCH_LENGTHS_TABLE_SIZE = 286;
    constexpr const uint32_t OFFSETS_TABLE_SIZE                = 30;
    constexpr const uint32_t MATCH_LENGTHS_TABLE_SIZE          = 255;

    namespace huff
    {
        struct code
        {
            uint16_t code;
            uint8_t  code_length;
        };
    }
}
#endif //DPP_DEFS_HPP
