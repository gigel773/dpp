#ifndef DPP_HASH_SERVICE_IMPL_HPP
#define DPP_HASH_SERVICE_IMPL_HPP

#include <cstdint>
#include "hash_service.hpp"

namespace dpp::compressors::deflate::internal
{

    class HashCrc32 : public IHashService<uint32_t, uint8_t>
    {
    public:
        uint32_t calculate(uint8_t *data) override;
    };

}
#endif //DPP_HASH_SERVICE_IMPL_HPP
