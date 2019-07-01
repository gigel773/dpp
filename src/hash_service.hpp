#ifndef DPP_HASH_SERVICE_HPP
#define DPP_HASH_SERVICE_HPP

#include <memory>

namespace dpp::compressors::deflate::internal
{
    template<class IndexType, class DataType>
    class IHashService
    {
    public:
        using Ptr = std::shared_ptr<IHashService<IndexType, DataType>>;

        virtual IndexType calculate(const DataType *data) = 0;

        static Ptr getHashCrc32Service();
    };
}

#endif //DPP_HASH_SERVICE_HPP
