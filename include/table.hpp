#ifndef DPP_TABLE_HPP
#define DPP_TABLE_HPP

#include <map>
#include <vector>
#include <memory>
#include "statistics.hpp"

namespace dpp::tables
{

    template<class Symbol, class Code>
    class ITable
    {
    public:
        virtual ITable &build(std::unique_ptr<IStatistics<Symbol>> m_statisticService) = 0;

        virtual Code get(const Symbol &symbol) = 0;
    };

}

#endif //DPP_TABLE_HPP
