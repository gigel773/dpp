#include "deflate_compressor.hpp"
#include "deflate_service.hpp"

dpp::compressors::deflate::DeflateCompressor::DeflateCompressor() : m_source(nullptr)
{
    m_histogram = dpp::tables::deflate::getDeflateHistogram();
    m_table     = dpp::tables::deflate::getDeflateHuffmanTable();
}

void dpp::compressors::deflate::DeflateCompressor::compress()
{
    // Variables
    auto matchDiscoveryService = dpp::deflate::internal::IMatchDiscoveryService::getMatchDiscoveryService();

    // Gather statistics and build Huffman table
    m_histogram->gather(m_source);
    m_table->build(m_histogram);

    // Encode input source
    dpp::deflate::internal::deflatePass(m_source,
                                        matchDiscoveryService,
                                        [](uint8_t symbol) -> void
                                        {
                                            // Symbol encoding actions
                                        },
                                        [](dpp::deflate::internal::Match match) -> void
                                        {
                                            // Match encoding actions
                                        });
}

void dpp::compressors::deflate::DeflateCompressor::setSource(uint8_t *source)
{
    m_source = source;
}

dpp::compressors::ICompressor::CompressorPtr dpp::compressors::getDeflateCompressor()
{
    return std::make_shared<dpp::compressors::deflate::DeflateCompressor>();
}
