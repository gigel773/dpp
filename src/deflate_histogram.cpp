#include "deflate_histogram.hpp"
#include "deflate_service.hpp"

namespace dpp::tables
{
    constexpr uint32_t LITERALS_MATCH_LENGTHS_TABLE_SIZE = 286;
    constexpr uint32_t OFFSETS_TABLE_SIZE                = 30;

    deflate::Histogram::Histogram()
    {
        m_literalsMatchLengthsTable.reserve(LITERALS_MATCH_LENGTHS_TABLE_SIZE);
        m_offsetsTable.reserve(OFFSETS_TABLE_SIZE);
    }

    dpp::tables::IStatistics<uint8_t> &deflate::Histogram::gather(const uint8_t *source, uint32_t sourceSize)
    {
        // Variables
        auto matchDiscoveryService = compressors::deflate::internal::IMatchDiscoveryService::getMatchDiscoveryService();
        auto hashService           = compressors::deflate::internal::IHashService<uint32_t, uint8_t>::getHashCrc32Service();
        auto hashTable             = tables::deflate::internal::IHashTable<uint32_t, uint32_t>::getDefaultHashTable();

        // Configuring match discovery service
        matchDiscoveryService->setHashService(hashService);
        matchDiscoveryService->setHashTable(hashTable);

        // Main actions
        dpp::compressors::deflate::internal::deflatePass(source,
                                                         sourceSize,
                                                         matchDiscoveryService,
                                                         [this](const uint8_t symbol)
                                                         {
                                                             m_literalsMatchLengthsTable[symbol]++;
                                                         },
                                                         [this](const dpp::compressors::deflate::internal::Match match)
                                                         {
                                                             m_literalsMatchLengthsTable[match.getLengthIndex()]++;
                                                             m_offsetsTable[match.getOffsetIndex()]++;
                                                         });

        return *this;
    }

    uint32_t deflate::Histogram::getSymbolFrequency(uint8_t symbol)
    {
        return 0;
    }

    deflate::Histogram::Ptr deflate::Histogram::getDeflateHistogram()
    {
        return std::make_shared<dpp::tables::deflate::Histogram>();
    }
}
