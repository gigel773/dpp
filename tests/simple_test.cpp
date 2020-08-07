#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <memory>

#include <dpp.hpp>

#if WIN32
constexpr char testFilePath[] = R"(..\..\testdata\bib)";
#else
constexpr char testFilePath[] = R"(../../testdata/bib)";
#endif

int main()
{
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

    std::vector<uint8_t> destination(result.size(), 0);

    auto result_size = dpp::deflate(std::begin(result), std::end(result), std::begin(destination));

    std::cout << "Compression ratio: " << (float) (result.size()) / (float) (result_size) << "\n";

    return 0;
}
