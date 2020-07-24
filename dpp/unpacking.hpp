#ifndef DPP_UNPACKING_HPP
#define DPP_UNPACKING_HPP

#include "defs.hpp"

namespace dpp
{
    auto create_match_lengths_mapping(const std::array<huff::code, LITERALS_MATCH_LENGTHS_TABLE_SIZE> &alphabet)
    -> std::array<huff::code, 258>
    {
        constexpr uint32_t          bits_to_step[] = {1, 3, 7, 15, 31};
        std::array<huff::code, 258> result{};

        uint8_t  current_bit_length = 0;
        uint32_t current_length     = 3;
        uint32_t current_base       = 256;

        // Complete initial part
        while (current_base < 264)
        {
            result[current_length - 3] = alphabet[current_base];

            current_base++;
            current_length++;
        }

        // Complete main part
        while (current_length - 3 < result.size() && current_bit_length < 5)
        {
            for (int32_t interval = 0;
                 interval < 4 && (current_length - 3) < result.size();
                 interval++, current_base++)
            {
                const uint32_t upper_boundary = current_length + bits_to_step[current_bit_length];
                uint8_t        current_bits   = 0;

                while (current_length <= upper_boundary && (current_length - 3) < result.size())
                {
                    // TODO implement bit reverse
                    const uint16_t new_code        = alphabet[current_base].code |
                                                     (current_bits << alphabet[current_base].code_length);
                    const uint8_t  new_code_length = alphabet[current_base].code_length + current_bit_length + 1;

                    result[current_length - 3] = {new_code, new_code_length};
                    current_bits++;
                    current_length++;
                }
            }

            current_bit_length++;
        }

        // Complete 258 length
        result[current_base + 1] = alphabet.back();

        return result;
    }
}

#endif //DPP_UNPACKING_HPP
