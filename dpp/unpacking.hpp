#ifndef DPP_UNPACKING_HPP
#define DPP_UNPACKING_HPP

#include "defs.hpp"

namespace dpp
{
    struct mapping_t
    {
        uint8_t extra_bits;
        uint8_t extra_bits_count;
    };


    auto create_match_lengths_mapping() -> std::array<mapping_t, 258>
    {
        constexpr uint32_t         bits_to_step[] = {1, 3, 7, 15, 31};
        std::array<mapping_t, 258> result{};

        uint8_t  current_bit_length = 0;
        uint32_t current_length     = 11;

        while (current_length - 3 < result.size() && current_bit_length < 5)
        {
            for (int32_t interval = 0; interval < 4 && (current_length - 3) < result.size(); interval++)
            {
                const uint32_t upper_boundary = current_length + bits_to_step[current_bit_length];
                uint8_t        current_bits   = 0;

                while (current_length <= upper_boundary && (current_length - 3) < result.size())
                {
                    result[current_length - 3] = {current_bits,
                                                  static_cast<uint8_t>(current_bit_length + 1)};
                    current_bits++;
                    current_length++;
                }
            }

            current_bit_length++;
        }

        return result;
    }
}

#endif //DPP_UNPACKING_HPP
