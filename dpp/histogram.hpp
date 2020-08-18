#ifndef DPP_HISTOGRAM_HPP
#define DPP_HISTOGRAM_HPP

#include <defs.hpp>
#include <util.hpp>

namespace dpp
{
    class histogram_t
    {
    public:
        struct match_t
        {
            int32_t length;
            int32_t offset;
        };

        constexpr histogram_t()
        {
            literals_matches_histogram_[256] = 1;
        }

        void update(const match_t &match) noexcept
        {
            literals_matches_histogram_[util::get_length_index(match.length)]++;
            offsets_histogram_[util::get_offset_index(match.offset)]++;
        }

        template<class type_t,
                class = typename std::enable_if<sizeof(type_t) == 1>::type>
        void update(type_t literal)
        {
            literals_matches_histogram_[literal]++;
        }

        [[nodiscard]] auto get_literals_histogram() const noexcept
        -> const std::array<int16_t, LITERALS_MATCH_LENGTHS_TABLE_SIZE> &
        {
            return literals_matches_histogram_;
        }

        [[nodiscard]] auto get_offsets_histogram() const noexcept
        -> const std::array<int16_t, OFFSETS_TABLE_SIZE> &
        {
            return offsets_histogram_;
        }

    private:
        std::array<int16_t, LITERALS_MATCH_LENGTHS_TABLE_SIZE> literals_matches_histogram_{};
        std::array<int16_t, OFFSETS_TABLE_SIZE>                offsets_histogram_{};
    };
}

#endif //DPP_HISTOGRAM_HPP
