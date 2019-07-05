#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

#include <compressor.hpp>

constexpr char testFilePath[] = R"(..\..\testdata\bib)";

int main()
{
    auto          deflateCompressor = dpp::compressors::getDeflateCompressor();
    std::ifstream file(testFilePath, std::ifstream::binary);
    std::string   result;

    if (!file.is_open())
    {
        return 1;
    }

    file.seekg(0, std::ios::end);
    result.reserve(static_cast<unsigned int>(file.tellg()));
    file.seekg(0, std::ios::beg);

    result.assign(std::istreambuf_iterator<char>(file),
                  std::istreambuf_iterator<char>());

    deflateCompressor->setSource(reinterpret_cast<const uint8_t *>(result.c_str()), result.length());
    deflateCompressor->compress();

    return 0;
}
