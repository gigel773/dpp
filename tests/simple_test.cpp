#include <iostream>
#include <fstream>
#include <array>
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

    std::array<int16_t, dpp::huff::MAX_ALPHABET_SIZE>         histogram{};
    std::array<dpp::huff::code, dpp::huff::MAX_ALPHABET_SIZE> alphabet{};

    for (auto c : result)
    {
        histogram[c]++;
    }

    dpp::huff::build_huffman_alphabet(histogram, alphabet);

    uint32_t bit_length = 0;

    for (auto c : result)
    {
        bit_length += alphabet[c].code_length;
    }

    uint32_t byte_length = (bit_length / 8) + (bit_length % 8 ? 1 : 0);

    std::cout << "Resulted byte length: " << byte_length << "\n";
    std::cout << "Compression ratio: " << (float) result.size() / (float) byte_length << "\n";

    return 0;
}
