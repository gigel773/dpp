#ifndef DPP_BIT_WRITER_HPP
#define DPP_BIT_WRITER_HPP

namespace dpp
{
    namespace util
    {
        auto build_mask(uint8_t bit_count) -> uint64_t
        {
            return (1u << bit_count) - 1;
        }
    }

    class bit_stream
    {
    public:
        template<class iterator_t>
        explicit bit_stream(iterator_t begin)
                : buffer_(0),
                  begin_(&*begin),
                  cur_output_(&*begin),
                  bits_left_(8)
        {
            // Empty constructor
        }

        void write(uint8_t bits, uint8_t bit_count, bool inverse = false)
        {
            if (inverse)
            {
                bits = util::reverse_t(bits) >> (8u - bit_count);
            }

            const uint8_t bit_difference = bit_count - bits_left_;

            if (bit_count < bits_left_)
            {
                buffer_ |= (bits & util::build_mask(bit_count)) << (8u - bits_left_);
                bits_left_ -= bit_count;
            } else if (bit_count == bits_left_)
            {
                buffer_ |= (bits & util::build_mask(bit_count)) << (8u - bits_left_);
                bits_left_ = 8;

                *cur_output_ = buffer_;
                buffer_ = 0;
                cur_output_++;
            } else
            {
                // Write as many as possible
                buffer_ |= (bits & util::build_mask(bits_left_)) << (8u - bits_left_);

                *cur_output_ = buffer_;
                buffer_ = 0;
                cur_output_++;

                // Write rest
                uint8_t new_bits = bits >> bits_left_;
                buffer_ |= new_bits & util::build_mask(bit_difference);

                bits_left_ = 8 - bit_difference;
            }
        }

        void write(uint16_t bits, uint8_t bit_count, bool inverse = false)
        {
            if (inverse)
            {
                bits = util::reverse_t(bits) >> (16u - bit_count);
            }

            const size_t full_bytes = bit_count / 8;
            const size_t last_bits  = bit_count % 8;
            const auto   *ptr       = reinterpret_cast<uint8_t *>(&bits);

            for (size_t i = 0; i < full_bytes; i++)
            {
                bit_stream::write(*ptr, 8);
                ptr++;
            }

            bit_stream::write(*ptr, last_bits);
        }

        void write(uint32_t bits, uint8_t bit_count, bool inverse = false)
        {
            if (inverse)
            {
                bits = util::reverse_t(bits) >> (32u - bit_count);
            }

            const size_t full_words = bit_count / 16;
            const size_t last_bits  = bit_count % 16;
            const auto   *ptr       = reinterpret_cast<uint16_t *>(&bits);

            for (size_t i = 0; i < full_words; i++)
            {
                bit_stream::write(*ptr, 16);
                ptr++;
            }

            bit_stream::write(*ptr, last_bits);
        }

        void flush()
        {
            *cur_output_ = buffer_;
            buffer_    = 0;
            bits_left_ = 8;
            cur_output_++;
        }

        auto get_written_bytes() -> size_t
        {
            return cur_output_ - begin_;
        }

    private:
        uint8_t       buffer_;
        const uint8_t *begin_;
        uint8_t       *cur_output_;
        uint8_t       bits_left_;
    };
}

#endif //DPP_BIT_WRITER_HPP
