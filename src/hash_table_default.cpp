#include <cstring>

#include "hash_table_default.hpp"

namespace dpp::tables::deflate::internal
{
    HashTableDefault::HashTableDefault()
            : m_hashTable(),
              m_mask(DEFAULT_HASH_TABLE_MASK)
    {
        memset(m_hashTable, UNINITIALIZED_INDEX, DEFAULT_HASH_TABLE_SIZE);
    }

    uint32_t HashTableDefault::get(const uint32_t &value)
    {
        return m_hashTable[value] & m_mask;
    }

    void HashTableDefault::update(const uint32_t &index, const uint32_t &value)
    {
        m_hashTable[value & m_mask] = index;
    }

    template<>
    HashTableDefault::Ptr IHashTable<uint32_t, uint32_t>::getDefaultHashTable()
    {
        return std::make_shared<HashTableDefault>();
    }
}