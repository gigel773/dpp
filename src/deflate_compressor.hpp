#ifndef DPP_DEFLATE_COMPRESSOR_HPP
#define DPP_DEFLATE_COMPRESSOR_HPP

#include "compressor.hpp"
#include "deflate_histogram.hpp"
#include "huffman_table.hpp"

namespace dpp::compressors::deflate
{
    class DeflateCompressor : public ICompressor
    {
    public:
        using HistogramPtr = std::shared_ptr<dpp::tables::deflate::Histogram>;
        using TablePtr = std::shared_ptr<dpp::tables::deflate::HuffmanTable>;

        DeflateCompressor();

        void compress() override;

        void setSource(uint8_t *source) override;

    private:
        HistogramPtr m_histogram;
        TablePtr     m_table;
        uint8_t      *m_source;
    };
}

#endif //DPP_DEFLATE_COMPRESSOR_HPP
