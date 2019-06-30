#ifndef DPP_DEFLATE_HISTOGRAM_HPP
#define DPP_DEFLATE_HISTOGRAM_HPP

#include <vector>

#include "statistics.hpp"

namespace dpp::tables::deflate
{

    class Histogram : public IStatistics<uint8_t>
    {
    public:
        using Ptr =std::shared_ptr<Histogram>;

        IStatistics<uint8_t> &gather(uint8_t *source) override;

        uint32_t getSymbolFrequency(uint8_t symbol) override;

        static Ptr getDeflateHistogram();
    };
}

#endif //DPP_DEFLATE_HISTOGRAM_HPP
