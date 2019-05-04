#ifndef DEFLATE___COMPRESSORS_HPP
#define DEFLATE___COMPRESSORS_HPP

#include <functional>
#include <utility>

namespace compressors {

    template<class ReturnType>
    class ICompressor {
        using CallbackType = std::function<ReturnType(const ICompressor &)>;

    public:
        virtual ReturnType compress() = 0;

        ICompressor &setCallback(CallbackType callback) {
            m_callback = std::move(callback);
            return *this;
        }

    private:
        CallbackType m_callback;
        uint32_t     m_windowSize;
    };

}

#endif //DEFLATE___COMPRESSORS_HPP
