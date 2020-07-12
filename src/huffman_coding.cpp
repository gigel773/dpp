#include <vector>
#include <algorithm>

#include "huffman_coding.hpp"

namespace dpp::huff
{

    namespace internal
    {
        auto calculate_code_length(const tree_node &node,
                                   const std::vector<tree_node> &tree,
                                   const uint8_t current_length,
                                   code mapping[MAX_ALPHABET_SIZE]) -> void
        {
            if (node.left_child == -1 && node.right_child == -1)
            {
                mapping[node.character].code_length = current_length;
            }

            if (node.left_child != -1)
            {
                calculate_code_length(tree[node.left_child], tree, current_length + 1, mapping);
            }

            if (node.right_child != -1)
            {
                calculate_code_length(tree[node.right_child], tree, current_length + 1, mapping);
            }
        }
    }

    void build_tree(const int16_t *histogram, std::vector<dpp::huff::tree_node> &huffman_tree)
    {
        std::vector<heap_node> heap;

        huffman_tree.reserve(255 * 2);
        heap.reserve(255);

        // Prepare a tree and heap
        for (uint8_t i = 0; i < 255; i++)
        {
            if (0 == histogram[i])
            {
                continue;
            }

            huffman_tree.push_back({.character = i, .left_child = -1, .right_child = -1});
            heap.push_back({
                                   .alphabet_index = static_cast<int16_t>(huffman_tree.size() - 1),
                                   .frequency = histogram[i]
                           });
        }

        // Initial heap building
        auto heap_begin = std::begin(heap);
        auto heap_end = std::end(heap);

        std::make_heap(heap_begin, heap_end, heap_node_comparator);

        // Building a Huffman tree
        while (std::distance(heap_begin, heap_end) > 1)
        {
            /*
             * To build a Huffman tree we should:
             *
             * 1. Find two least elements (with smallest frequencies)
             * 2. Create new node with frequency equal to sum of two least frequencies
             * 3. Add new node to the heap
             * */
            std::pop_heap(heap_begin, heap_end, heap_node_comparator);
            heap_node h1 = *(heap_end - 1);

            heap_end--;

            std::pop_heap(heap_begin, heap_end, heap_node_comparator);
            heap_node h2 = *(heap_end - 1);

            // Create parent for both data structures
            tree_node tree_parent = {
                    .character = INTERNAL_NODE,
                    .left_child = h1.alphabet_index,
                    .right_child = h2.alphabet_index
            };

            heap_node heap_parent = {
                    .alphabet_index = static_cast<int16_t>(huffman_tree.size()),
                    .frequency = static_cast<int16_t>(h1.frequency + h2.frequency)
            };

            // Update heap and tree
            huffman_tree.push_back(tree_parent);
            *(heap_end - 1) = heap_parent;

            std::make_heap(heap_begin, heap_end, heap_node_comparator);
        }
    }

    void calculate_codes(dpp::huff::code *alphabet)
    {
        uint8_t code_lengths[MAX_CODE_LENGTH] = {0};
        uint32_t next_code[MAX_CODE_LENGTH] = {0};
        uint32_t code = 0;

        // Calculate number of each code length
        for (int32_t i = 0; i < MAX_ALPHABET_SIZE; i++)
        {
            if (0 == alphabet[i].code_length)
            {
                continue;
            }

            code_lengths[alphabet[i].code_length]++;
        }

        // Calculate starting code for each code length (RFC 1951 algorithm)
        for (int32_t code_length = 1; code_length < MAX_CODE_LENGTH; code_length++)
        {
            code = (code + code_lengths[code_length - 1]) << 1u;
            next_code[code_length] = code;
        }

        // Generate codes themselves
        for (int32_t i = 0; i < MAX_ALPHABET_SIZE; i++)
        {
            if (0 == alphabet[i].code_length)
            {
                continue;
            }

            alphabet[i].code = next_code[alphabet[i].code_length];
            next_code[alphabet[i].code_length]++;
        }
    }

    auto calculate_code_lengths(const std::vector<tree_node> &tree, dpp::huff::code *mapping) -> void
    {
        tree_node root = tree.back();
        uint8_t code_length = 1;

        if (root.left_child != -1)
        {
            internal::calculate_code_length(tree[root.left_child], tree, code_length, mapping);
        }

        if (root.right_child != -1)
        {
            internal::calculate_code_length(tree[root.right_child], tree, code_length, mapping);
        }
    }
}
