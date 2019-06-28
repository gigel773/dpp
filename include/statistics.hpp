#ifndef DPP_STATISTICS_HPP
#define DPP_STATISTICS_HPP

#include <cstdint>
#include <memory>

namespace dpp::tables
{

    template<class Value, class StatisticsContainer>
    class IStatistics
    {
    public:
        IStatistics() = default;

        virtual ~IStatistics() = default;

        virtual IStatistics &gather() = 0;

        IStatistics &setCounter(const StatisticsContainer &&counter)
        {
            m_counter = counter;

            return *this;
        }

        IStatistics &setSource(const std::unique_ptr<Value> &source)
        {
            m_source = source;

            return *this;
        }

    private:
        StatisticsContainer    m_counter;
        std::unique_ptr<Value> m_source;
    };

}

#endif //DPP_STATISTICS_HPP
