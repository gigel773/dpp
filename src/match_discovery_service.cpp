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
            auto offset         = string - m_lowerBound + candidateIndex;

            if (candidateIndex == dpp::tables::deflate::internal::UNINITIALIZED_INDEX ||
                offset > MAXIMAL_OFFSET)
            {
                return Match(0, 0, candidateIndex);
            }

            auto matchLength = compareStrings(m_lowerBound + candidateIndex, string, 258);
            m_hashTable->update(string - m_lowerBound, hash);

            return Match(matchLength, offset, candidateIndex);
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

    Match::Match(uint32_t length, uint32_t offset, uint32_t index)
            : length(length),
              offset(offset),
              index(index)
    {
        // Empty constructor
    }

    bool Match::isMatch()
    {
        return length >= MINIMAL_MATCH_LENGTH;
    }

    uint32_t Match::getLengthIndex() const
    {
        if (length < 11)
        {
            return 257 + length - 3;
        } else if (length < 19)
        {
            return 261 + (length - 3) / 2;
        } else if (length < 35)
        {
            return 265 + (length - 3) / 4;
        } else if (length < 67)
        {
            return 269 + (length - 3) / 8;
        } else if (length < 131)
        {
            return 273 + (length - 3) / 16;
        } else if (length < 258)
        {
            return 277 + (length - 3) / 32;
        } else
        {
            return 285;
        }
    }

    uint32_t Match::getOffsetIndex() const
    {
        if (offset <= 2)
        {
            return offset - 1;
        } else if (offset <= 4)
        {
            return 0 + (offset - 1) / 1;
        } else if (offset <= 8)
        {
            return 2 + (offset - 1) / 2;
        } else if (offset <= 16)
        {
            return 4 + (offset - 1) / 4;
        } else if (offset <= 32)
        {
            return 6 + (offset - 1) / 8;
        } else if (offset <= 64)
        {
            return 8 + (offset - 1) / 16;
        } else if (offset <= 128)
        {
            return 10 + (offset - 1) / 32;
        } else if (offset <= 256)
        {
            return 12 + (offset - 1) / 64;
        } else if (offset <= 512)
        {
            return 14 + (offset - 1) / 128;
        } else if (offset <= 1024)
        {
            return 16 + (offset - 1) / 256;
        } else if (offset <= 2048)
        {
            return 18 + (offset - 1) / 512;
        } else if (offset <= 4096)
        {
            return 20 + (offset - 1) / 1024;
        } else if (offset <= 8192)
        {
            return 22 + (offset - 1) / 2048;
        } else if (offset <= 16384)
        {
            return 24 + (offset - 1) / 4096;
        } else if (offset <= 32768)
        {
            return 26 + (offset - 1) / 8192;
        } else
        {
            return ~0u;    /* ~0 is an invalid distance code */
        }
    }
}
