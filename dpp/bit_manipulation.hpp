#ifndef DPP_BIT_MANIPULATION_HPP
#define DPP_BIT_MANIPULATION_HPP

namespace dpp::util
{
    constexpr auto reverse(uint8_t value) -> uint8_t
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

    constexpr auto build_reverse_table() -> std::array<uint8_t, std::numeric_limits<uint8_t>::max() + 1>
    {
        std::array<uint8_t, std::numeric_limits<uint8_t>::max() + 1> result{};

        for (uint32_t i = 0; i < result.size(); i++)
        {
            result[i] = reverse(static_cast<uint8_t>(i));
        }

        return result;
    }

    auto reverse_t(uint8_t byte) -> uint8_t
    {
        static constexpr const auto table = build_reverse_table();

        return table[byte];
    }

    auto reverse_t(uint16_t value) -> uint16_t
    {
        uint8_t result[2];
        auto    *result_ptr = reinterpret_cast<uint16_t *>(result);

        *(result_ptr) = value;

        result[0] = reverse_t(result[0]);
        result[1] = reverse_t(result[1]);

        std::swap(result[0], result[1]);

        return *result_ptr;
    }
}

#endif //DPP_BIT_MANIPULATION_HPP
