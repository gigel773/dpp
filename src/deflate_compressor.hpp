#ifndef DPP_DEFLATE_COMPRESSOR_HPP
#define DPP_DEFLATE_COMPRESSOR_HPP

#include "compressor.hpp"
#include "huffman_table.hpp"

namespace dpp::compressors::deflate
{

    class DeflateCompressor : public ICompressor
    {
    public:
        void compress() override;

    private:
        dpp::tables::deflate::HuffmanTable m_table;
    };
}

#endif //DPP_DEFLATE_COMPRESSOR_HPP
