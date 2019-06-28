#include "match_discovery_service.hpp"

class MatchDiscoveryService : public dpp::deflate::internal::IMatchDiscoveryService
{

};

dpp::deflate::internal::IMatchDiscoveryService
dpp::deflate::internal::IMatchDiscoveryService::getMatchDiscoveryService()
{
    MatchDiscoveryService matchDiscoveryService;
    return matchDiscoveryService;
}
