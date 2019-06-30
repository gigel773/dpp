#ifndef DPP_HASH_SERVICE_HPP
#define DPP_HASH_SERVICE_HPP

namespace dpp::compressors::deflate::internal
{
    template<class IndexType, class DataType>
    class IHashService
    {
    public:
        virtual IndexType calculate(DataType *data) = 0;
    };
}

#endif //DPP_HASH_SERVICE_HPP
