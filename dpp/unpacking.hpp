#ifndef DPP_UNPACKING_HPP
#define DPP_UNPACKING_HPP

#include "defs.hpp"

namespace dpp
{
    struct mapping_t
    {
        uint8_t base_index;
        uint8_t extra_bits;
        uint8_t extra_bits_count;
    };

    constexpr std::array<mapping_t, 258> create_match_lengths_mapping()
    {
        std::array<mapping_t, 258> result{};

        return result;
    }
}

#endif //DPP_UNPACKING_HPP
