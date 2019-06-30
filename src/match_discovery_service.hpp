#ifndef DPP_MATCH_DISCOVERY_SERVICE_HPP
#define DPP_MATCH_DISCOVERY_SERVICE_HPP

namespace dpp::compressors::deflate::internal
{
    class IMatchDiscoveryService
    {
    public:
        static IMatchDiscoveryService getMatchDiscoveryService();
    };
}

#endif //DPP_MATCH_DISCOVERY_SERVICE_HPP
