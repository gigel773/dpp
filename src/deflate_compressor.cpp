#include "deflate_compressor.hpp"

void dpp::compressors::deflate::DeflateCompressor::compress()
{

}

void dpp::compressors::deflate::DeflateCompressor::setSource(uint8_t *source)
{
    m_source = source;
}

dpp::compressors::deflate::DeflateCompressor::DeflateCompressor() : m_source(nullptr)
{
    // Empty constructor
}

dpp::compressors::ICompressor::CompressorPtr dpp::compressors::getDeflateCompressor()
{
    return std::make_shared<dpp::compressors::deflate::DeflateCompressor>();
}
