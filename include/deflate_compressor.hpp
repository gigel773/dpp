#ifndef DEFLATE___DEFLATE_COMPRESSOR_HPP
#define DEFLATE___DEFLATE_COMPRESSOR_HPP

#include <compressors.hpp>

namespace compressors::deflate {

    template<class ReturnType>
    class DeflateCompressor : public ICompressor<ReturnType> {

    };

}

#endif //DEFLATE___DEFLATE_COMPRESSOR_HPP
