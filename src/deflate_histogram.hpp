#ifndef DPP_DEFLATE_HISTOGRAM_HPP
#define DPP_DEFLATE_HISTOGRAM_HPP

#include <vector>

#include "statistics.hpp"

namespace dpp::tables::deflate
{

    class Histogram : public IStatistics<uint8_t>
    {
    public:
        IStatistics<uint8_t> &gather() override;

        uint32_t getSymbolFrequency(uint8_t symbol) override;
    };
}

#endif //DPP_DEFLATE_HISTOGRAM_HPP
