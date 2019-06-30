#include "match_discovery_service.hpp"

namespace dpp::compressors::deflate::internal
{

    class MatchDiscoveryService : public IMatchDiscoveryService
    {
    public:
        MatchDiscoveryService() = default;

        Match getBestMatch(const uint8_t *string) override
        {
            return Match();
        }
    };

    IMatchDiscoveryService::Ptr IMatchDiscoveryService::getMatchDiscoveryService()
    {
        return std::make_shared<MatchDiscoveryService>();
    }

}
