#include "deflate_histogram.hpp"

namespace dpp::tables
{

    dpp::tables::IStatistics<uint8_t> &deflate::Histogram::gather(const uint8_t *source)
    {
        return *this;
    }

    uint32_t deflate::Histogram::getSymbolFrequency(uint8_t symbol)
    {
        return 0;
    }

    deflate::Histogram::Ptr deflate::Histogram::getDeflateHistogram()
    {
        return std::make_shared<dpp::tables::deflate::Histogram>();
    }
}
