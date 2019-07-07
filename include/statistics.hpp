#ifndef DPP_STATISTICS_HPP
#define DPP_STATISTICS_HPP

#include <cstdint>
#include <memory>

namespace dpp::tables
{
    template<class Symbol>
    class IStatistics
    {
    public:
        IStatistics() = default;

        virtual ~IStatistics() = default;

        virtual IStatistics &gather(const Symbol *source, uint32_t sourceSize) = 0;

        virtual uint32_t getSymbolFrequency(Symbol symbol) = 0;
    };

}

#endif //DPP_STATISTICS_HPP
