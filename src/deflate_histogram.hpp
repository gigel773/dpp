#ifndef DPP_DEFLATE_HISTOGRAM_HPP
#define DPP_DEFLATE_HISTOGRAM_HPP

#include <vector>

#include "statistics.hpp"

namespace dpp::tables::deflate
{

    class Histogram : public IStatistics<uint8_t, std::vector<uint32_t>>
    {
    public:
        IStatistics<uint8_t, std::vector<uint32_t>> &gather() override;
    };
}

#endif //DPP_DEFLATE_HISTOGRAM_HPP
