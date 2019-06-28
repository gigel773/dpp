//
// Created by Марк Лемешевский on 2019-06-29.
//

#ifndef DPP_MATCH_DISCOVERY_SERVICE_HPP
#define DPP_MATCH_DISCOVERY_SERVICE_HPP


namespace dpp::deflate::internal
{
    class IMatchDiscoveryService
    {
    public:
        static IMatchDiscoveryService getMatchDiscoveryService();
    };
}


#endif //DPP_MATCH_DISCOVERY_SERVICE_HPP
