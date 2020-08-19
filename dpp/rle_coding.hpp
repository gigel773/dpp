#ifndef DPP_RLE_CODING_HPP
#define DPP_RLE_CODING_HPP

#include "defs.hpp"

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
            class callable_t,
            class element_accessor_t>
    void run(const input_iterator_t begin,
             const input_iterator_t end,
             callable_t instruction_handler,
             element_accessor_t accessor)
    {
        static_assert(std::is_invocable<callable_t, instruction>::value,
                      "Wrong instruction handler passed");
        static_assert(std::is_same<typename std::result_of<element_accessor_t(
                              typename std::iterator_traits<input_iterator_t>::value_type)>::type, uint8_t>::value,
                      "Wrong accessor passed");

        auto   cur_symbol    = begin;
        auto   next_symbol   = cur_symbol + 1;
        size_t element_count = 0;

        while (next_symbol < end)
        {
            if (accessor(*cur_symbol) == accessor(*next_symbol))
            {
                element_count++;
            } else
            {
                instruction_handler(instruction(accessor(*cur_symbol), element_count));
                element_count = 0;
            }

            cur_symbol++;
            next_symbol++;
        }

        instruction_handler(instruction(accessor(*cur_symbol), element_count));
    }
}

#endif //DPP_RLE_CODING_HPP
