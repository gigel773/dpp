#ifndef DPP_DEFLATE_HISTOGRAM_HPP
#define DPP_DEFLATE_HISTOGRAM_HPP

#include "interfaces/statistics.hpp"

namespace dpp::tables::deflate {

    template<class Value, class Counter>
    class Histogram : public IStatistics<Value, Counter> {
    public:
        IStatistics<Value, Counter> &gather() override {
            return *this;
        }
    };

}

#endif //DPP_DEFLATE_HISTOGRAM_HPP
