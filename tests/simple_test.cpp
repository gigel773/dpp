#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <memory>
#include <filesystem>

#include <dpp.hpp>

#if WIN32
constexpr char testFilePath[] = R"(..\..\testdata)";
#else
constexpr char testFilePath[] = R"(../../testdata)";
#endif

int main()
{
    for (auto &path: std::filesystem::directory_iterator(testFilePath))
    {
        std::ifstream file(path.path().string(), std::ifstream::binary);
        std::string   result;

        if (!file.is_open())
        {
            return 1;
        }

        result.reserve(path.file_size());

        result.assign(std::istreambuf_iterator<char>(file),
                      std::istreambuf_iterator<char>());

        std::vector<uint8_t> destination(result.size(), 0);

        auto result_size = dpp::deflate(std::begin(result), std::end(result), std::begin(destination));

        std::cout << "\n[\t" << path.path().filename() << "\t]\n";
        std::cout << "Compressed size: " << result_size << "\n";
        std::cout << "Compression ratio: " << (float) (result.size()) / (float) (result_size) << "\n";
        std::cout << "Compression %: " << ((float) (result_size) / (float) (result.size())) * 100 << "\n";
    }
    
    return 0;
}
