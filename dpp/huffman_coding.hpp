#ifndef DPP_HUFFMAN_CODING_HPP
#define DPP_HUFFMAN_CODING_HPP

#include "defs.hpp"

namespace dpp::huff
{
    struct tree_h_node
    {
        int32_t frequency;
        int16_t left_child;
        int16_t right_child;
    };

    struct code
    {
        uint16_t code;
        uint8_t  code_length;
    };

    constexpr const uint32_t MAX_CODE_LENGTH                   = 16;
    constexpr const uint32_t LITERALS_MATCH_LENGTHS_TABLE_SIZE = 286;
    constexpr const uint32_t OFFSETS_TABLE_SIZE                = 30;

    template<uint32_t table_size_t>
    void build_huffman_alphabet(const std::array<int16_t, table_size_t> &histogram,
                                std::array<code, table_size_t> &huffman_alphabet);
}


#endif //DPP_HUFFMAN_CODING_HPP
