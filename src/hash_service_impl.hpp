#ifndef DPP_HASH_SERVICE_IMPL_HPP
#define DPP_HASH_SERVICE_IMPL_HPP

#include <cstdint>
#include "hash_service.hpp"

namespace dpp::compressors::deflate::internal
{

    class HashCrc32 : public IHashService<uint32_t, uint8_t>
    {
    public:
        using Ptr = IHashService<uint32_t, uint8_t>::Ptr;

        uint32_t calculate(const uint8_t *data) override;
    };

}
#endif //DPP_HASH_SERVICE_IMPL_HPP
