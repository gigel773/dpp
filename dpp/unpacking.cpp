#include "unpacking.hpp"

namespace dpp
{
    namespace util
    {
        constexpr auto reverse_byte(uint8_t value) -> uint8_t
        {
            uint8_t result       = 0;
            uint8_t bit_position = 1u << 7u;

            for (int32_t i = 0; i < 8; i++)
            {
                if (value & 1u)
                {
                    result |= bit_position;
                }

                bit_position >>= 1u;
                value >>= 1u;
            }

            return result;
        }

        constexpr auto build_reverse_table() -> std::array<uint8_t, 256>
        {
            std::array<uint8_t, 256> result{};

            for (uint16_t i = 0; i < 256; i++)
            {
                result[i] = reverse_byte(static_cast<uint8_t>(i));
            }

            return result;
        }

        auto reverse_byte_table(uint8_t byte) -> uint8_t
        {
            static constexpr const auto table = build_reverse_table();

            return table[byte];
        }
    }

    auto unpack_match_lengths_table(const std::array<huff::code, LITERALS_MATCH_LENGTHS_TABLE_SIZE> &alphabet)
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
                const uint32_t upper_boundary    = current_length + bits_to_step[current_bit_length];
                const uint8_t  actual_bit_length = current_bit_length + 1;
                uint8_t        current_bits      = 0;

                while (current_length <= upper_boundary && (current_length - 3) < result.size())
                {
                    const uint16_t code            = alphabet[current_base].code;
                    const uint16_t code_length     = alphabet[current_base].code_length;
                    const uint8_t  reversed_bits   = util::reverse_byte_table(current_bits) >> (8u - actual_bit_length);
                    const uint16_t new_code        = code | static_cast<uint16_t>(reversed_bits << code_length);
                    const uint8_t  new_code_length = code_length + actual_bit_length;

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