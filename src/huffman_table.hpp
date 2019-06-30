#ifndef DPP_HUFFMAN_TABLE_HPP
#define DPP_HUFFMAN_TABLE_HPP

#include <unordered_map>

#include "table.hpp"

namespace dpp::tables::deflate
{
    class HuffmanTable : public ITable<uint8_t, uint16_t>
    {
    public:
        using Ptr = std::shared_ptr<HuffmanTable>;
        using StatisticsPtr = std::shared_ptr<IStatistics<uint8_t>>;

        ITable<uint8_t, uint16_t> &build(StatisticsPtr m_statisticService) override;

        uint16_t get(const uint8_t &symbol) override;

        static Ptr getDeflateHuffmanTable();

    private:
        std::unordered_map<uint8_t, uint32_t> m_table;
    };
}

#endif //DPP_HUFFMAN_TABLE_HPP
