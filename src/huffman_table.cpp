#include "huffman_table.hpp"

dpp::tables::ITable<uint8_t, uint16_t> &dpp::tables::deflate::HuffmanTable::build(StatisticsPtr m_statisticService)
{
    return *this;
}

uint16_t dpp::tables::deflate::HuffmanTable::get(const uint8_t &symbol)
{
    return m_table[symbol];
}

std::shared_ptr<dpp::tables::deflate::HuffmanTable> dpp::tables::deflate::getDeflateHuffmanTable()
{
    return std::make_shared<dpp::tables::deflate::HuffmanTable>();
}
