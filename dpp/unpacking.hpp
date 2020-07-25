#ifndef DPP_UNPACKING_HPP
#define DPP_UNPACKING_HPP

#include "defs.hpp"

namespace dpp
{
    auto create_match_lengths_mapping(const std::array<huff::code, LITERALS_MATCH_LENGTHS_TABLE_SIZE> &alphabet)
    -> std::array<huff::code, 258>;
}

#endif //DPP_UNPACKING_HPP
