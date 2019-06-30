#include "match_discovery_service.hpp"

namespace dpp::compressors::deflate::internal
{

    class MatchDiscoveryService : public IMatchDiscoveryService
    {

    };

    IMatchDiscoveryService IMatchDiscoveryService::getMatchDiscoveryService()
    {
        MatchDiscoveryService matchDiscoveryService;
        return matchDiscoveryService;
    }

}
