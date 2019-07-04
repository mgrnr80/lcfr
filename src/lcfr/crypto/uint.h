#pragma once

#include <stdint.h>
#include <string.h>
#include "lcfr/arch/endianness.h"
#include "lcfr/crypto/mp_arithmetic.h"

namespace lcfr {

/** Structure representing a fixed size big unsigned integer.
* The integers are represented as arrays of primitive unsigned integer words (least significant word before).
* Template parameters:
* - NB: bit size of the numbers (must be a multiple of the word size)
* - W: primitive unsigned integer type
*/
template <unsigned NB, class W = uint32_t>
struct alignas(16) ui
{
    static const unsigned WO = sizeof(W);
    static const unsigned WB = 8 * WO;
    static const unsigned NW = (NB + WB - 1) / WB;
    static const unsigned NO = (NB + 7) / 8;
    static const unsigned NX = (NB + 3) / 4;

    W digits[NW];

    ui() {}

    ui(W x)
    {
        for (size_t i = 0; i < NW; i++) digits[i] = 0;
        digits[0] = x;
    }

    ui(typename uint_traits<W>::s x)
    {
        auto init = x < 0 ? W(-1) : W(0);
        for (size_t i = 0; i < NW; i++) digits[i] = init;
        digits[0] = x;
    }

    ui(const W* x)
    {
        for (size_t i = 0; i < NW; i++) digits[i] = x[i];
    }

    ui(const W* x, size_t n)
    {
        size_t m = n < NW ? n : NW;
        for (size_t i = 0; i < m; i++) digits[i] = x[i];
        for (size_t i = m; i < NW; i++) digits[i] = W(0);
    }

    ui(const uint8_t* x, size_t n)
    {
        size_t m = n < NO ? n : NO;
        size_t w = m / WO;
        for (size_t i = 0; i < w; i++) be_deserialize(digits[i], x + n - (i + 1) * WO);
        lcfr::zero(digits + w, NW - w);
        for (size_t j = w * WO, k = 0; j < m; j++, k++) digits[w] |= x[n - j - 1] << (k * 8);
    }

    ui(const ui& x)
    {
        for (size_t i = 0; i < NW; i++) digits[i] = x.digits[i];
    }

    ui(const char* hex)
    {
        size_t n = strlen(hex);
        for (size_t i = 0; i < NW; i++)
        {
            W x = W(0);
            for (size_t j = 0; j < WB / 4; j++)
            {
                size_t k = i * WB / 4 + j;
                if (k < n) x |= parse_hex_digit(hex[n - k - 1]) << (j * 4);
            }
            digits[i] = x;
        }
    }

    operator W* () { return digits; }
    operator const W* () const { return digits; }

    size_t word_count()
    {
        for (size_t i = NW; i > 0; i--) if (digits[i - 1] != W(0)) return i;
        return 0;
    }

    ui& operator = (const ui& x)
    {
        for (size_t i = 0; i < NW; i++) digits[i] = x.digits[i];
        return *this;
    }

    ui& operator &= (const ui& x)
    {
        for (size_t i = 0; i < NW; i++) digits[i] &= x.digits[i];
        return *this;
    }

    void swap(ui& x)
    {
        for (size_t i = 0; i < NW; i++) 
        {
            auto t = digits[i];
            digits[i] = x.digits[i];
            x.digits[i] = t;
        }
    }

    bool operator == (const ui& x) const { return eq(digits, x, NW); }
    bool operator != (const ui& x) const { return !eq(digits, x, NW); }

    void to_hex(char* str) const
    {
        //if (n < (NB / 4)) throw std::runtime_error();
        size_t n = NB / 4;
        for (size_t i = 0; i < NW; i++)
        {
            for (size_t j = 0; j < WB / 4; j++)
            {
                size_t k = i * WB / 4 + j;
                str[n - k - 1] = hex_digit(digits[i] >> (j * 4));
            }
        }
    }

    void to_bytes(uint8_t* a, size_t n)
    {
        size_t nw = n / WO < NW ? n / WO : NW;
        size_t no = (n < NO ? n : NO) % WO;
        for (size_t i = 0; i < nw; i++) be_serialize(a + n - (i + 1) * WO, digits[i]);
        for (size_t j = 0; j < no; j++) a[n - nw * WO - j - 1] = (uint8_t)(W(0xff) & (digits[nw] >> (j * 8)));
        for (size_t k = NO; k < n; k++) a[n - k - 1] = 0;
    }

    static ui ones(size_t nb)
    {
        ui x;
        for (size_t i = 0; i < nb / WB; i++) x[i] = W(-1);
        size_t k = nb / WB; size_t r = nb % WB;
        if (k < NW) x[k] = (W(1) << r) - W(1);
        for (size_t i = k + 1; i < NW; i++) x[i] = W(0);
        return x;
    }

    static W parse_hex_digit(char hex)
    {
        if ((hex <= '9') && (hex >= '0')) return W(hex) - W(48);
        if ((hex <= 'f') && (hex >= 'a')) return W(hex) - W(87);
        if ((hex <= 'F') && (hex >= 'A')) return W(hex) - W(55);
        return W(0);
    }

    static char hex_digit(W x)
    {
        x &= W(0xf);
        if (x < 10) return '0' + uint8_t(x);
        else        return 'A' + uint8_t(x - W(10));
    }

    static const ui ZERO;
    static const ui ONE;
};

template <unsigned NB, class W> const ui<NB, W> ui<NB, W>::ZERO(W(0));
template <unsigned NB, class W> const ui<NB, W> ui<NB, W>::ONE(W(1));

}
