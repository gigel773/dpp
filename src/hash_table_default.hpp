#ifndef DPP_HASH_TABLE_DEFAULT_HPP
#define DPP_HASH_TABLE_DEFAULT_HPP

#include "hash_table.hpp"

namespace dpp::tables::deflate::internal
{

    constexpr uint32_t DEFAULT_HASH_TABLE_SIZE = 32768;
    constexpr uint32_t DEFAULT_HASH_TABLE_MASK = DEFAULT_HASH_TABLE_SIZE - 1;

    class HashTableDefault : public IHashTable<uint32_t, uint32_t>
    {
    public:
        using Ptr = IHashTable<uint32_t, uint32_t>::Ptr;

        HashTableDefault();

        uint32_t get(const uint32_t &value) override;

        void update(const uint32_t &index, const uint32_t &value) override;

    private:
        uint32_t m_hashTable[DEFAULT_HASH_TABLE_SIZE];
        uint32_t m_mask;
    };

}
#endif //DPP_HASH_TABLE_DEFAULT_HPP
