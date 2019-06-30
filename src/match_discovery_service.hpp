#ifndef DPP_MATCH_DISCOVERY_SERVICE_HPP
#define DPP_MATCH_DISCOVERY_SERVICE_HPP

#include <cstdint>
#include <memory>

namespace dpp::compressors::deflate::internal
{
    constexpr uint32_t minimalMatchLength = 4;

    struct Match
    {
        uint32_t length;
        uint32_t offset;
        uint32_t index;

        bool isMatch()
        {
            return length >= minimalMatchLength;
        }
    };

    class IMatchDiscoveryService
    {
    public:
        using Ptr = std::shared_ptr<IMatchDiscoveryService>;

        virtual Match getBestMatch(const uint8_t *string) = 0;

        static Ptr getMatchDiscoveryService();
    };
}

#endif //DPP_MATCH_DISCOVERY_SERVICE_HPP
