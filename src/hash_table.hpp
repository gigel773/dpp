#ifndef DPP_HASH_TABLE_HPP
#define DPP_HASH_TABLE_HPP

#include <memory>

namespace dpp::tables::deflate::internal
{

    constexpr uint32_t UNINITIALIZED_INDEX = 0xFFFFFFFF;

    template<class IndexType, class ValueType>
    class IHashTable
    {
    public:
        using Ptr = std::shared_ptr<IHashTable<IndexType, ValueType>>;

        virtual IndexType get(const ValueType &value) = 0;

        virtual void update(const IndexType &index, const ValueType &value) = 0;

        static Ptr getDefaultHashTable();
    };

}
#endif //DPP_HASH_TABLE_HPP
