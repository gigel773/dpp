#ifndef DPP_TABLE_HPP
#define DPP_TABLE_HPP

#include <map>
#include <vector>
#include <memory>
#include "interfaces/statistics.hpp"

namespace dpp::tables {

    template<class Symbol, class Code>
    class ITable {
    public:
        virtual ITable &build() = 0;

        virtual Code get(const Symbol &symbol) = 0;

    private:
        std::unique_ptr<IStatistics<Symbol, std::vector<uint32_t>>> m_statisticService;
    };

}

#endif //DPP_TABLE_HPP
