#include "deflate_histogram.hpp"

dpp::tables::IStatistics<uint8_t> &dpp::tables::deflate::Histogram::gather()
{
    return *this;
}

uint32_t dpp::tables::deflate::Histogram::getSymbolFrequency(uint8_t symbol)
{
    return 0;
}
