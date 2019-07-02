#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

#include <compressor.hpp>

constexpr char testFilePath[] = "../testdata/bib";

int main()
{
    auto          deflateCompressor = dpp::compressors::getDeflateCompressor();
    std::ifstream file(testFilePath);
    auto          finishPosition    = file.tellg();
    auto          content           = std::make_unique<char[]>(static_cast<uint32_t >(finishPosition));

    file.seekg(0, std::ios::beg);
    file.read(content.get(), finishPosition);

    deflateCompressor->setSource(reinterpret_cast<uint8_t *>(content.get()), finishPosition);
    deflateCompressor->compress();

    return 0;
}
