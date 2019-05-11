#ifndef DPP_DEFLATE_COMPRESSOR_HPP
#define DPP_DEFLATE_COMPRESSOR_HPP

#include "interfaces/compressor.hpp"
#include "deflate_histogram.hpp"

namespace dpp::compressors::deflate {

    template<class ReturnType>
    class DeflateCompressor : public ICompressor<ReturnType> {
    private:
        dpp::tables::deflate::Histogram<uint8_t, std::vector<uint32_t>> m_histogram;
    };

}

#endif //DPP_DEFLATE_COMPRESSOR_HPP
