#ifndef DPP_HUFFMAN_CODING_HPP
#define DPP_HUFFMAN_CODING_HPP

#include "defs.hpp"

namespace dpp::huff
{
    struct code
    {
        uint16_t code;
        uint8_t  code_length;
    };

    template<uint32_t table_size_t>
    void build_huffman_alphabet(const std::array<int16_t, table_size_t> &histogram,
                                std::array<code, table_size_t> &huffman_alphabet);
}


#endif //DPP_HUFFMAN_CODING_HPP
