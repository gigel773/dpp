#include "huffman_table.hpp"

namespace dpp::tables
{

    ITable<uint8_t, uint16_t> &deflate::HuffmanTable::build(StatisticsPtr m_statisticService)
    {
        return *this;
    }

    uint16_t deflate::HuffmanTable::get(const uint8_t &symbol)
    {
        return m_table[symbol];
    }

    deflate::HuffmanTable::Ptr deflate::HuffmanTable::getDeflateHuffmanTable()
    {
        return std::make_shared<dpp::tables::deflate::HuffmanTable>();
    }
}
