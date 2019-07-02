#include "match_discovery_service.hpp"

namespace dpp::compressors::deflate::internal
{

    static inline uint32_t compareStrings(const uint8_t *const src1,
                                          const uint8_t *const src2,
                                          const uint32_t maxLength)
    {
        // Variables
        uint32_t matchLength = 0;

        if (src1 > src2)
        {
            return 0;
        }

        // Main cycle
        while (src1[matchLength] == src2[matchLength] &&
               src1 + matchLength < src2 &&
               matchLength <= maxLength)
        {
            matchLength++;
        }

        return matchLength;
    }

    class MatchDiscoveryService : public IMatchDiscoveryService
    {
    public:
        MatchDiscoveryService() = default;

        Match getBestMatch(const uint8_t *string) override
        {
            auto hash           = m_hashService->calculate(string);
            auto candidateIndex = m_hashTable->get(hash);

            if (candidateIndex == dpp::tables::deflate::internal::UNINITIALIZED_INDEX)
            {
                return Match(0, 0, candidateIndex);
            }

            auto matchLength = compareStrings(m_lowerBound + candidateIndex, string, 258);
            m_hashTable->update(string - m_lowerBound, hash);

            return Match(matchLength, string - m_lowerBound + candidateIndex, candidateIndex);
        }
    };

    IMatchDiscoveryService::Ptr IMatchDiscoveryService::getMatchDiscoveryService()
    {
        return std::make_shared<MatchDiscoveryService>();
    }

    void IMatchDiscoveryService::setHashService(const IHashService<uint32_t, uint8_t>::Ptr &hashService)
    {
        m_hashService = hashService;
    }

    void IMatchDiscoveryService::setHashTable(const IMatchDiscoveryService::HashTable::Ptr &hashTable)
    {
        m_hashTable = hashTable;
    }

    void IMatchDiscoveryService::setBounds(const uint8_t *lowerBound, const uint8_t *upperBound)
    {
        m_lowerBound = lowerBound;
        m_upperBound = upperBound;
    }
}
