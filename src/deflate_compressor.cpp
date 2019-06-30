#include "deflate_compressor.hpp"
#include "deflate_service.hpp"

namespace dpp::compressors
{

    deflate::DeflateCompressor::DeflateCompressor() :
            m_source(nullptr),
            m_sourceSize(0)
    {
        m_histogram = dpp::tables::deflate::Histogram::getDeflateHistogram();
        m_table     = dpp::tables::deflate::HuffmanTable::getDeflateHuffmanTable();
    }

    void deflate::DeflateCompressor::compress()
    {
        // Variables
        auto matchDiscoveryService = deflate::internal::IMatchDiscoveryService::getMatchDiscoveryService();

        // Gather statistics and build Huffman table
        m_histogram->gather(m_source);
        m_table->build(m_histogram);

        // Encode input source
        deflate::internal::deflatePass(m_source,
                                       m_sourceSize,
                                       matchDiscoveryService,
                                       [](uint8_t symbol)
                                       {
                                           // Symbol encoding actions
                                       },
                                       [](deflate::internal::Match match)
                                       {
                                           // Match encoding actions
                                       });
    }

    void deflate::DeflateCompressor::setSource(uint8_t *source, uint32_t sourceSize)
    {
        m_source     = source;
        m_sourceSize = sourceSize;
    }

    ICompressor::CompressorPtr getDeflateCompressor()
    {
        return std::make_shared<deflate::DeflateCompressor>();
    }
}