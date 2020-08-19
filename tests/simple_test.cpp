#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <memory>
#include <filesystem>

#include <dpp.hpp>

#include <isa-l.h>

#if WIN32
constexpr char testFilePath[] = R"(..\..\testdata)";
#else
constexpr char testFilePath[] = R"(../../testdata)";
#endif

int main()
{
    for (auto &path: std::filesystem::directory_iterator(testFilePath))
    {
        std::ifstream        file(path.path().string(), std::ifstream::binary);
        std::vector<uint8_t> source;
        std::vector<uint8_t> destination;
        std::vector<uint8_t> reference;

        size_t reference_size = 0;

        if (!file.is_open())
        {
            return 1;
        }

        source.reserve(path.file_size());
        destination.resize(path.file_size(), 0);
        reference.resize(path.file_size(), 0);

        source.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

        auto result_size = dpp::deflate(std::begin(source), std::end(source), std::begin(destination));

        std::cout << std::endl;
        inflate_state state{};

        isal_inflate_init(&state);

        state.next_in   = destination.data();
        state.avail_in  = result_size;
        state.next_out  = reference.data();
        state.avail_out = reference.size();

        auto status = isal_inflate_stateless(&state);

        if (status)
        {
            std::cout << "Decompression failed with: " << status << "\n";
            continue;
        }

        std::cout << "\n[ " << path.path().filename() << " ]\t";
        std::cout << "Compression ratio: " << (float) (source.size()) / (float) (result_size) << "\n";
    }

    return 0;
}
