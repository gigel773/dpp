#include <iostream>
#include <compressor.hpp>

int main()
{
    auto deflateCompressor = dpp::compressors::getDeflateCompressor();

    return 0;
}
