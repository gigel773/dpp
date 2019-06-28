#include "deflate_histogram.hpp"

dpp::tables::IStatistics<uint8_t, std::vector<uint32_t>> &dpp::tables::deflate::Histogram::gather()
{
    return *this;
}
