#ifndef DPP_COMPRESSORS_HPP
#define DPP_COMPRESSORS_HPP

#include <functional>
#include <utility>

namespace dpp::compressors
{

    constexpr uint32_t defaultWindowSize = 32768u;

    class ICompressor
    {
    public:
        using CallbackType = std::function<void(const ICompressor &)>;
        using CompressorPtr = std::shared_ptr<ICompressor>;

        ICompressor() : m_windowSize(defaultWindowSize)
        {
            // Empty default constructor
        }

        virtual void compress() = 0;

        virtual void setSource(uint8_t *source) = 0;

        ICompressor &setCallback(CallbackType callback)
        {
            m_callback = std::move(callback);
            return *this;
        }

    private:
        CallbackType m_callback;
        uint32_t     m_windowSize;
    };

    ICompressor::CompressorPtr getDeflateCompressor();
}

#endif //DPP_COMPRESSORS_HPP
