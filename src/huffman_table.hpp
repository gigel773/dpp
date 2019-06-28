#ifndef DPP_HUFFMAN_TABLE_HPP
#define DPP_HUFFMAN_TABLE_HPP

#include <unordered_map>

#include "table.hpp"
#include "deflate_histogram.hpp"

namespace dpp::tables::deflate
{
    class HuffmanTable : public ITable<uint8_t, uint16_t>
    {
    public:

        using StatisticsPtr = std::unique_ptr<IStatistics<uint8_t>>;

        ITable<uint8_t, uint16_t> &build(StatisticsPtr m_statisticService) override;

        uint16_t get(const uint8_t &symbol) override;

    private:
        dpp::tables::deflate::Histogram       m_histogram;
        std::unordered_map<uint8_t, uint32_t> m_table;
    };
}

#endif //DPP_HUFFMAN_TABLE_HPP
