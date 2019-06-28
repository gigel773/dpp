#ifndef DPP_DEFLATE_SERVICE_HPP
#define DPP_DEFLATE_SERVICE_HPP

#include "match_discovery_service.hpp"

namespace dpp::deflate::internal
{
    struct Match
    {
        uint32_t length;
        uint32_t offset;
        uint32_t index;
    };

    template<typename MatchDiscoveryService, typename LiteralsHandler, typename MatchHandler>
    void deflatePass(uint8_t *source,
                     MatchDiscoveryService matchDiscoveryService,
                     LiteralsHandler literalsHandler,
                     MatchHandler matchHandler)
    {
        // Check correctness of the passed parameters
        static_assert(std::is_base_of<IMatchDiscoveryService, MatchDiscoveryService>());
        static_assert(std::is_invocable<decltype(literalsHandler), uint8_t>::value);
        static_assert(std::is_invocable<decltype(matchHandler), Match>::value);
    }
}

#endif //DPP_DEFLATE_SERVICE_HPP
