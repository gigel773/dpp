#include "deflate_compressor.hpp"

void dpp::compressors::deflate::DeflateCompressor::compress()
{

}

dpp::compressors::ICompressor::CompressorPtr dpp::compressors::getDeflateCompressor()
{
    return std::make_shared<deflate::DeflateCompressor>();
}
