#ifndef DPP_MATCH_DISCOVERY_SERVICE_HPP
#define DPP_MATCH_DISCOVERY_SERVICE_HPP

#include <cstdint>
#include <memory>

#include "hash_service.hpp"
#include "hash_table.hpp"

namespace dpp::compressors::deflate::internal
{
    constexpr uint32_t minimalMatchLength = 4;

    struct Match
    {
        uint32_t length;
        uint32_t offset;
        uint32_t index;

        Match(uint32_t length, uint32_t offset, uint32_t index)
                : length(length),
                  offset(offset),
                  index(index)
        {
            // Empty constructor
        }

        bool isMatch()
        {
            return length >= minimalMatchLength;
        }
    };

    class IMatchDiscoveryService
    {
    public:
        using Ptr = std::shared_ptr<IMatchDiscoveryService>;
        using HashTable = dpp::tables::deflate::internal::IHashTable<uint32_t, uint32_t>;

        void setHashService(const IHashService<uint32_t, uint8_t>::Ptr &hashService);

        void setHashTable(const HashTable::Ptr &hashTable);

        void setBounds(const uint8_t *lowerBound, const uint8_t *upperBound);

        virtual Match getBestMatch(const uint8_t *string) = 0;

        static Ptr getMatchDiscoveryService();

    protected:
        IHashService<uint32_t, uint8_t>::Ptr m_hashService;
        HashTable::Ptr                       m_hashTable;
        const uint8_t                        *m_lowerBound;
        const uint8_t                        *m_upperBound;
    };
}

#endif //DPP_MATCH_DISCOVERY_SERVICE_HPP
