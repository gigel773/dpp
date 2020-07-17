#ifndef DPP_UTIL_HPP
#define DPP_UTIL_HPP

namespace dpp::util
{
    template<class iterator_t>
    auto compare(iterator_t left, iterator_t right) -> int32_t
    {
        const iterator_t boundary     = right;
        int32_t          match_length = 0;

        while (left < boundary &&
               *left == *right)
        {
            match_length++;
            left++;
            right++;
        }

        return match_length >= MINIMAL_MATCH_LENGTH ? match_length : -1;
    }

    auto get_length_index(int32_t length) -> uint32_t
    {
        if (length < 11)
        {
            return 257 + length - 3;
        } else if (length < 19)
        {
            return 261 + (length - 3) / 2;
        } else if (length < 35)
        {
            return 265 + (length - 3) / 4;
        } else if (length < 67)
        {
            return 269 + (length - 3) / 8;
        } else if (length < 131)
        {
            return 273 + (length - 3) / 16;
        } else if (length < 258)
        {
            return 277 + (length - 3) / 32;
        } else
        {
            return 285;
        }
    }

    auto get_offset_index(int32_t offset) -> uint32_t
    {
        if (offset <= 2)
        {
            return offset - 1;
        } else if (offset <= 4)
        {
            return 0 + (offset - 1) / 1;
        } else if (offset <= 8)
        {
            return 2 + (offset - 1) / 2;
        } else if (offset <= 16)
        {
            return 4 + (offset - 1) / 4;
        } else if (offset <= 32)
        {
            return 6 + (offset - 1) / 8;
        } else if (offset <= 64)
        {
            return 8 + (offset - 1) / 16;
        } else if (offset <= 128)
        {
            return 10 + (offset - 1) / 32;
        } else if (offset <= 256)
        {
            return 12 + (offset - 1) / 64;
        } else if (offset <= 512)
        {
            return 14 + (offset - 1) / 128;
        } else if (offset <= 1024)
        {
            return 16 + (offset - 1) / 256;
        } else if (offset <= 2048)
        {
            return 18 + (offset - 1) / 512;
        } else if (offset <= 4096)
        {
            return 20 + (offset - 1) / 1024;
        } else if (offset <= 8192)
        {
            return 22 + (offset - 1) / 2048;
        } else if (offset <= 16384)
        {
            return 24 + (offset - 1) / 4096;
        } else if (offset <= 32768)
        {
            return 26 + (offset - 1) / 8192;
        } else
        {
            return ~0u;    /* ~0 is an invalid distance code */
        }
    }
}

#endif //DPP_UTIL_HPP
