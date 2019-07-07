#ifndef DPP_DEFLATE_HISTOGRAM_HPP
#define DPP_DEFLATE_HISTOGRAM_HPP

#include <vector>

#include "statistics.hpp"

namespace dpp::tables::deflate
{

    class Histogram final : public IStatistics<uint8_t>
    {
    public:
        using Ptr =std::shared_ptr<Histogram>;

        Histogram();

        IStatistics<uint8_t> &gather(const uint8_t *source, uint32_t sourceSize) override;

        uint32_t getSymbolFrequency(uint8_t symbol) override;

        static Ptr getDeflateHistogram();

    private:
        std::vector<uint32_t> m_literalsMatchLengthsTable;
        std::vector<uint32_t> m_offsetsTable;
    };
}

#endif //DPP_DEFLATE_HISTOGRAM_HPP
