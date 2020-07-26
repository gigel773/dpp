#ifndef DPP_UNPACKING_HPP
#define DPP_UNPACKING_HPP

#include "defs.hpp"

namespace dpp
{
    auto unpack_match_lengths_table(const std::array<huff::code, LITERALS_MATCH_LENGTHS_TABLE_SIZE> &alphabet)
    -> std::array<huff::code, 258>;

    auto unpack_offsets_table(const std::array<huff::code, OFFSETS_TABLE_SIZE> &alphabet)
    -> std::array<huff::code, 32768>;
}

#endif //DPP_UNPACKING_HPP
