#include "hash_service_impl.hpp"

namespace dpp::compressors::deflate::internal
{
    uint32_t HashCrc32::calculate(const uint8_t *data)
    {
        return 0;
    }

    template<>
    HashCrc32::Ptr IHashService<uint32_t, uint8_t>::getHashCrc32Service()
    {
        return std::make_shared<HashCrc32>();
    }
}