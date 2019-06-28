#ifndef DPP_DEFLATE_COMPRESSOR_HPP
#define DPP_DEFLATE_COMPRESSOR_HPP

#include "compressor.hpp"
#include "huffman_table.hpp"

namespace dpp::compressors::deflate
{
    class DeflateCompressor : public ICompressor
    {
    public:
        DeflateCompressor();

        void compress() override;

        void setSource(uint8_t *source) override;

    private:
        dpp::tables::deflate::HuffmanTable m_table;
        uint8_t                            *m_source;
    };
}

#endif //DPP_DEFLATE_COMPRESSOR_HPP
