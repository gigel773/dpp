#include "huffman_table.hpp"

dpp::tables::ITable<uint8_t, uint16_t> &dpp::tables::deflate::HuffmanTable::build()
{
    return *this;
}

uint16_t dpp::tables::deflate::HuffmanTable::get(const uint8_t &symbol)
{
    return m_table[symbol];
}
