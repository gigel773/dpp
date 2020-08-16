#ifndef DPP_RLE_CODING_HPP
#define DPP_RLE_CODING_HPP

namespace dpp::rle
{
    struct instruction
    {
        instruction(uint8_t symbol, size_t count)
                : symbol(symbol), count(count)
        {
            // Default
        }

        uint8_t symbol;
        size_t  count;
    };

    template<class input_iterator_t,
            class callable_t>
    void encode(const input_iterator_t begin,
                const input_iterator_t end,
                callable_t instruction_handler)
    {
        static_assert(std::is_invocable<callable_t, instruction>::value,
                      "Wrong instruction handler passed");

        auto   cur_symbol    = begin;
        auto   next_symbol   = cur_symbol + 1;
        size_t element_count = 1;

        while (next_symbol < end)
        {
            if (*cur_symbol == *next_symbol)
            {
                element_count++;
            } else
            {
                instruction_handler(instruction(cur_symbol->code_length, element_count));
                element_count = 1;
            }

            cur_symbol++;
            next_symbol++;
        }

        instruction_handler(instruction(cur_symbol->code_length, element_count));
    }
}

#endif //DPP_RLE_CODING_HPP
