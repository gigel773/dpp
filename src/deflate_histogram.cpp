#include "deflate_histogram.hpp"

dpp::tables::IStatistics<uint8_t> &dpp::tables::deflate::Histogram::gather(uint8_t *source)
{
    return *this;
}

uint32_t dpp::tables::deflate::Histogram::getSymbolFrequency(uint8_t symbol)
{
    return 0;
}

std::shared_ptr<dpp::tables::deflate::Histogram> dpp::tables::deflate::getDeflateHistogram()
{
    return std::make_shared<dpp::tables::deflate::Histogram>();
}
