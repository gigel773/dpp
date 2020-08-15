#include <array>
#include <vector>
#include <algorithm>
#include <numeric>

#include "huffman_coding.hpp"

namespace dpp::huff
{
    struct tree_node_t
    {
        int32_t frequency;
        int16_t left_child;
        int16_t right_child;
    };

    struct package_node_t
    {
        constexpr package_node_t() = default;

        explicit package_node_t(int16_t value, bool is_packed = false)
                : value(value),
                  is_packed(is_packed)
        {
            // Default constructor
        }

        int16_t value     = 0;
        bool    is_packed = false;
    };

    namespace internal
    {
        template<class input_iterator_t, class output_iterator_t>
        void pack(input_iterator_t begin, input_iterator_t end, output_iterator_t dest)
        {
            if (std::distance(begin, end) % 2)
            {
                end--;
            }

            auto first    = begin;
            auto second   = begin + 1;
            auto cur_dest = dest;

            while (second < end)
            {
                *cur_dest++ = package_node_t(first->value + second->value, true);

                first += 2;
                second += 2;
            }
        }

        template<uint32_t table_size_t>
        static inline auto calculate_code_lengths(const std::array<int16_t, table_size_t> &histogram,
                                                  std::array<code, table_size_t> &alphabet) -> void
        {
            constexpr const size_t max_huffman_code = MAX_CODE_LENGTH;

            std::array<std::vector<package_node_t>, max_huffman_code> packages{};

            size_t count = std::count_if(std::begin(histogram), std::end(histogram), [](auto it)
            {
                return it > 0;
            });

            const size_t         package_count = count * 2 - 2;
            std::vector<int16_t> index_mapping;
            index_mapping.reserve(count);

            auto comparator = [](package_node_t &a, package_node_t &b)
            {
                return a.value < b.value;
            };

            auto current_package = std::begin(packages);
            auto next_package    = current_package + 1;

            for (size_t i = 0; i < max_huffman_code; i++)
            {
                packages[i].reserve(package_count);
            }

            for (size_t i = 0; i < table_size_t; i++)
            {
                if (0 == histogram[i])
                {
                    continue;
                }

                packages[0].emplace_back(histogram[i]);
                index_mapping.push_back(i);
            }

            std::sort(std::begin(packages[0]), std::end(packages[0]), comparator);
            std::sort(std::begin(index_mapping), std::end(index_mapping), [&histogram](auto a, auto b)
            {
                return histogram[a] < histogram[b];
            });

            while (next_package < std::end(packages))
            {
                std::vector<package_node_t> packed(std::min(package_count - count, current_package->size() / 2));

                auto cur_begin = current_package->begin();
                auto cur_end   = cur_begin + packed.size() * 2;

                pack(cur_begin, cur_end, std::begin(packed));

                std::copy(std::begin(packages[0]), std::end(packages[0]), std::back_inserter(*next_package));
                std::copy(packed.begin(), packed.end(), std::back_inserter(*next_package));

                std::sort(next_package->begin(), next_package->end(), comparator);

                // Next iteration
                current_package++;
                next_package++;
            }

            current_package = std::end(packages) - 2;
            auto previous_package = std::end(packages) - 1;

            while (current_package > std::begin(packages) - 1)
            {
                const size_t nodes_count = std::count_if(previous_package->begin(),
                                                         previous_package->end(),
                                                         [](package_node_t &node)
                                                         {
                                                             return node.is_packed;
                                                         });

                auto   element_count = std::min(nodes_count * 2, current_package->size());
                size_t element_pos   = 0;

                current_package->resize(element_count);

                for (auto &it: *current_package)
                {
                    if (!it.is_packed)
                    {
                        int16_t restored_idx = index_mapping[element_pos++];
                        alphabet[restored_idx].code_length++;
                    }
                }

                // Next iteration
                current_package--;
                previous_package--;
            }

            size_t element_pos = 0;

            for (auto &it: packages.back())
            {
                if (!it.is_packed)
                {
                    int16_t restored_idx = index_mapping[element_pos++];
                    alphabet[restored_idx].code_length++;
                }
            }
        }

        template<uint32_t table_size_t>
        static inline void calculate_codes(std::array<code, table_size_t> &alphabet)
        {
            uint8_t  code_lengths[MAX_CODE_LENGTH] = {0};
            uint32_t next_code[MAX_CODE_LENGTH]    = {0};
            uint32_t code                          = 0;

            // Calculate number of each code length
            for (int32_t i = 0; i < table_size_t; i++)
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
            for (int32_t i = 0; i < table_size_t; i++)
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

    template<uint32_t table_size_t>
    void build_huffman_alphabet(const std::array<int16_t, table_size_t> &histogram,
                                std::array<code, table_size_t> &huffman_alphabet)
    {
        internal::calculate_code_lengths<table_size_t>(histogram, huffman_alphabet);
        internal::calculate_codes<table_size_t>(huffman_alphabet);
    }

    template
    void build_huffman_alphabet<LITERALS_MATCH_LENGTHS_TABLE_SIZE>(
            const std::array<int16_t, LITERALS_MATCH_LENGTHS_TABLE_SIZE> &histogram,
            std::array<code, LITERALS_MATCH_LENGTHS_TABLE_SIZE> &huffman_alphabet);

    template
    void build_huffman_alphabet<OFFSETS_TABLE_SIZE>(const std::array<int16_t, OFFSETS_TABLE_SIZE> &histogram,
                                                    std::array<code, OFFSETS_TABLE_SIZE> &huffman_alphabet);
}
