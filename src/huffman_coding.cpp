#include <array>
#include <vector>
#include <algorithm>

#include "huffman_coding.hpp"

namespace dpp::huff
{
    namespace internal
    {
        static inline auto calculate_code_length(const std::array<tree_h_node, MAX_ALPHABET_SIZE * 2> &tree,
                                                 const int16_t node_idx,
                                                 const uint8_t current_length,
                                                 std::array<code, MAX_ALPHABET_SIZE> &huffman_alphabet) -> void
        {
            const auto &node = tree[node_idx];

            if (node.left_child == -1 && node.right_child == -1)
            {
                huffman_alphabet[node_idx].code_length = current_length;
            }

            if (node.left_child != -1)
            {
                calculate_code_length(tree, node.left_child, current_length + 1, huffman_alphabet);
            }

            if (node.right_child != -1)
            {
                calculate_code_length(tree, node.right_child, current_length + 1, huffman_alphabet);
            }
        }

        static inline auto calculate_code_lengths(const std::array<tree_h_node, MAX_ALPHABET_SIZE * 2> &tree,
                                                  int16_t root_node,
                                                  std::array<code, MAX_ALPHABET_SIZE> &huffman_alphabet)
        {
            const auto &node = tree[root_node];

            if (node.left_child != -1)
            {
                internal::calculate_code_length(tree, node.left_child, 1, huffman_alphabet);
            }

            if (node.right_child != -1)
            {
                internal::calculate_code_length(tree, node.right_child, 1, huffman_alphabet);
            }
        }

        static inline auto build_huffman_tree(const std::array<int16_t, MAX_ALPHABET_SIZE> &histogram,
                                              std::array<tree_h_node, MAX_ALPHABET_SIZE * 2> &tree) -> uint16_t
        {
            std::array<int16_t, MAX_ALPHABET_SIZE> heap{};

            auto heap_begin         = std::begin(heap);
            auto heap_next_position = heap_begin;
            auto tree_next_node     = std::begin(tree) + MAX_ALPHABET_SIZE;
            auto comparator         = [&tree](const int16_t &a, const int16_t &b) -> bool
            {
                return tree[a].frequency > tree[b].frequency;
            };

            // Copy histogram into the heap
            for (int16_t i = 0; i < MAX_ALPHABET_SIZE; i++)
            {
                if (0 == histogram[i])
                {
                    continue;
                }

                tree[i].frequency   = histogram[i];
                tree[i].right_child = -1;
                tree[i].left_child  = -1;

                *heap_next_position++ = i;
            }

            auto heap_end = heap_next_position;

            // Initialize heap
            std::make_heap(heap_begin, heap_end, comparator);

            // Build a tree
            while (std::distance(heap_begin, heap_end) > 1)
            {
                std::pop_heap(heap_begin, heap_end, comparator);
                auto lowest_idx1 = *(heap_end - 1);

                heap_end--;

                std::pop_heap(heap_begin, heap_end, comparator);
                auto lowest_idx2 = *(heap_end - 1);

                tree_next_node->frequency   = tree[lowest_idx1].frequency + tree[lowest_idx2].frequency;
                tree_next_node->left_child  = lowest_idx1;
                tree_next_node->right_child = lowest_idx2;

                *(heap_end - 1) = std::distance(std::begin(tree), tree_next_node);
                std::make_heap(heap_begin, heap_end, comparator);

                tree_next_node++;
            }

            return heap.front();
        }

        static inline void calculate_codes(std::array<code, MAX_ALPHABET_SIZE> &alphabet)
        {
            uint8_t  code_lengths[MAX_CODE_LENGTH] = {0};
            uint32_t next_code[MAX_CODE_LENGTH]    = {0};
            uint32_t code                          = 0;

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
    }

    void build_huffman_alphabet(const std::array<int16_t, MAX_ALPHABET_SIZE> &histogram,
                                std::array<code, MAX_ALPHABET_SIZE> &huffman_alphabet)
    {
        std::array<tree_h_node, MAX_ALPHABET_SIZE * 2> tree{};

        const uint16_t root_idx = internal::build_huffman_tree(histogram, tree);

        internal::calculate_code_lengths(tree, root_idx, huffman_alphabet);
        internal::calculate_codes(huffman_alphabet);
    }
}
