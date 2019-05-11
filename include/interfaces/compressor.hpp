#ifndef DPP_COMPRESSORS_HPP
#define DPP_COMPRESSORS_HPP

#include <functional>
#include <utility>

namespace dpp::compressors {

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

#endif //DPP_COMPRESSORS_HPP
