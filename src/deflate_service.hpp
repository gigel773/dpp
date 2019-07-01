#ifndef DPP_DEFLATE_SERVICE_HPP
#define DPP_DEFLATE_SERVICE_HPP

#include "match_discovery_service.hpp"

namespace dpp::compressors::deflate::internal
{
    template<typename MatchDiscoveryService, typename LiteralsHandler, typename MatchHandler>
    void deflatePass(const uint8_t *const source,
                     const uint32_t sourceSize,
                     const MatchDiscoveryService matchDiscoveryService,
                     const LiteralsHandler literalsHandler,
                     const MatchHandler matchHandler)
    {
        // Check correctness of the passed parameters
        static_assert(std::is_base_of<IMatchDiscoveryService::Ptr, MatchDiscoveryService>());
        static_assert(std::is_invocable<decltype(literalsHandler), uint8_t>::value);
        static_assert(std::is_invocable<decltype(matchHandler), Match>::value);

        // Variables
        const uint8_t *const lowerBound = source;
        const uint8_t *current = source;
        const uint8_t *const upperBound = source + sourceSize;

        matchDiscoveryService->setBounds(lowerBound, upperBound);

        // Main deflate pass routine
        while (current < upperBound)
        {
            auto match = matchDiscoveryService->getBestMatch(current);

            if (match.isMatch())
            {
                matchHandler(match);
                current += match.length;
            } else
            {
                literalsHandler(*current);
                current++;
            }
        }
    }
}

#endif //DPP_DEFLATE_SERVICE_HPP
