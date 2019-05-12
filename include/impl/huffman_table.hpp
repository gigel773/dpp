#ifndef DPP_HUFFMAN_TABLE_HPP
#define DPP_HUFFMAN_TABLE_HPP

#include "interfaces/table.hpp"
#include "deflate_histogram.hpp"

namespace dpp::tables::deflate {

    template<class Symbol, class Code>
    class HuffmanTable : public ITable<Symbol, Code> {
    public:
        ITable<Symbol, Code> &build() override {
            return *this;
        }

        Code get(const Symbol &symbol) override {
            return nullptr;
        }

    private:
        dpp::tables::deflate::Histogram<uint8_t, std::vector<uint32_t>> m_histogram;
    };

}

#endif //DPP_HUFFMAN_TABLE_HPP
