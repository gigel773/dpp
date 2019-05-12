#ifndef DPP_DEFLATE_COMPRESSOR_HPP
#define DPP_DEFLATE_COMPRESSOR_HPP

#include "interfaces/compressor.hpp"
#include "huffman_table.hpp"

namespace dpp::compressors::deflate {

    template<class ReturnType>
    class DeflateCompressor : public ICompressor<ReturnType> {
    private:
        dpp::tables::deflate::HuffmanTable<uint8_t, uint16_t> m_table;
    };

}

#endif //DPP_DEFLATE_COMPRESSOR_HPP
