#ifndef DPP_STATISTICS_HPP
#define DPP_STATISTICS_HPP

#include <cstdint>
#include <memory>

namespace dpp::tables {

    template<class Value, class Counter>
    class IStatistics {
    public:
        virtual IStatistics &gather() = 0;

        IStatistics &setCounter(const Counter &&counter) {
            m_counter = counter;

            return *this;
        }

        IStatistics &setSource(const std::unique_ptr<Value> &&source) {
            m_source = source;

            return *this;
        }

    private:
        Counter                m_counter;
        std::unique_ptr<Value> m_source;
    };

}

#endif //DPP_STATISTICS_HPP
