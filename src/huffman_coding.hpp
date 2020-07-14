#ifndef DPP_HUFFMAN_CODING_HPP
#define DPP_HUFFMAN_CODING_HPP

#include <cstdint>
#include <type_traits>

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

    constexpr const uint32_t MAX_ALPHABET_SIZE = 256;
    constexpr const uint32_t MAX_CODE_LENGTH   = 16;

    void build_huffman_alphabet(const std::array<int16_t, MAX_ALPHABET_SIZE> &histogram,
                                std::array<code, MAX_ALPHABET_SIZE> &huffman_alphabet);
}


#endif //DPP_HUFFMAN_CODING_HPP
