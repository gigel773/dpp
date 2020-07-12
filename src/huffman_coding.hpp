#ifndef DPP_HUFFMAN_CODING_HPP
#define DPP_HUFFMAN_CODING_HPP

#include <cstdint>
#include <type_traits>

namespace dpp::huff
{
    struct heap_node
    {
        int16_t alphabet_index;
        int16_t frequency;
    };

    struct tree_node
    {
        uint8_t character;
        int16_t left_child;
        int16_t right_child;
    };

    struct code
    {
        uint16_t code;
        uint8_t code_length;
    };

    constexpr const uint32_t MAX_ALPHABET_SIZE = 255;
    constexpr const uint32_t MAX_CODE_LENGTH = 16;
    constexpr const uint8_t INTERNAL_NODE = ' ';
    constexpr const uint16_t MAX_HEAP_SIZE = 255 * 3;

    constexpr auto heap_node_comparator = [](const heap_node &a, const heap_node &b) -> bool
    {
        return a.frequency > b.frequency;
    };

    auto calculate_code_lengths(const std::vector<tree_node> &tree, code mapping[MAX_ALPHABET_SIZE]) -> void;

    void build_tree(const int16_t histogram[MAX_ALPHABET_SIZE], std::vector<tree_node> &huffman_tree);

    void calculate_codes(code alphabet[MAX_ALPHABET_SIZE]);
}


#endif //DPP_HUFFMAN_CODING_HPP
