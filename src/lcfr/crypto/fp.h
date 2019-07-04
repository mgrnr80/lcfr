#pragma once

#include "lcfr/crypto/mp_arithmetic.h"
#include "lcfr/crypto/uint.h"

// https://primes.utm.edu/lists/2small/0bit.html
// largest n-bit primes:
//    16: 15, 17, 39, 57, 87, 89, 99, 113, 117, 123
//    24: 3, 17, 33, 63, 75, 77, 89, 95, 117, 167
//    32: 5, 17, 65, 99, 107, 135, 153, 185, 209, 267
//    40: 87, 167, 195, 203, 213, 285, 293, 299, 389, 437
//    48: 59, 65, 89, 93, 147, 165, 189, 233, 243, 257
//    56: 5, 27, 47, 57, 89, 93, 147, 177, 189, 195
//    64: 59, 83, 95, 179, 189, 257, 279, 323, 353, 363
//    80: 65, 93, 117, 143, 285, 317, 549, 645, 765, 933
//    96: 17, 87, 93, 147, 165, 189, 237, 243, 315, 347
//   112: 75, 189, 269, 297, 327, 519, 537, 623, 699, 875
//   128: 159, 173, 233, 237, 275, 357, 675, 713, 797, 1193
//   160: 47, 57, 75, 189, 285, 383, 465, 543, 659, 843
//   192: 237, 333, 399, 489, 527, 663, 915, 945, 1059, 1143
//   224: 63, 363, 573, 719, 773, 857, 1025, 1223, 1227, 1253
//   256: 189, 357, 435, 587, 617, 923, 1053, 1299, 1539, 1883
//   320: 197, 743, 825, 843, 873, 1007, 1017, 1217, 1815, 2955

namespace lcfr {

class small_fp
{
    uint32_t prime_;

public:
    small_fp(uint32_t prime)
        : prime_(prime)
    {}

    size_t getPrimeBitCount() const
    {
        return 32;
    }

    const uint32_t* getPrime() const
    {
        return &prime_;
    }

    void add(uint32_t* x, const uint32_t* a, const uint32_t* b) const
    {
        auto s = uint64_t(*a) + uint64_t(*b);
        if (s >= prime_) s -= prime_;
        *x = uint32_t(s);
    }

    void sub(uint32_t* x, const uint32_t* a, const uint32_t* b) const
    {
        auto s = int64_t(*a) - int64_t(*b);
        if (s < 0) s += prime_;
        *x = uint32_t(s);
    }

    void mult(uint32_t* x, const uint32_t* a, const uint32_t* b) const
    {
        auto p = uint64_t(*a) * uint64_t(*b);
        *x = uint32_t(p % prime_);
    }

    void half(uint32_t* x, const uint32_t* u) const
    {
        *x = (*u >> 1) + (*u % 1) * ((prime_ >> 1) + 1);
    }

    void twice(uint32_t* x, const uint32_t* u) const
    {
        add(x, u, u);
    }

    void square(uint32_t* x, const uint32_t* a) const
    {
        mult(x, a, a);
    }

    void inverse(uint32_t* x, const uint32_t* a) const
    {
        *x = lcfr::inverse(*a, prime_);
    }

    void modulo(uint32_t* x, const uint32_t* a, size_t na) const
    {
        if (na == 2)
        {
            auto p = uint64_t(a[0]) | (uint64_t(a[1]) << 32);
            *x = uint32_t(p % prime_);
        }
        else
        {
            *x = uint32_t(*a % prime_);
        }
    }
};

/** Class implementig an integer-modulus-prime finite field.
  The integers are represented as array of primitive unsigned integers (least significant word before),
  the size in bit of the array equal to the bit size of the prime.
* Template parameters are:
* - NP: bit size of the prime number
* - W: primitive unsigned integer type used to implement big unsigned integers
*/
template <unsigned NP, class W = uint32_t>
class pw_fp
{
    typedef typename uint_traits<W>::s SW;

    static const unsigned WB = 8 * sizeof(W);
    static const unsigned NW = (NP + WB - 1) / WB;
    static const unsigned NB = NW * WB;
    static const bool     FW = NP == NB;

    ui<NB, W>             prime_;
    ui<NB, W>             two_pow_;
    ui<NB, W>             half_prime_;
    ui<NB, W>             m_;
    ui<NB, W>             r_; // ~ m + m^2 + m^3
    size_t                nm_;
    size_t                nr_;

    void init_m_from_prime()
    {
        lcfr::shift_left(two_pow_, ui<NB, W>::ONE, size_t(NP), size_t(NW));
        lcfr::sub(m_, two_pow_, prime_, size_t(NW), size_t(NW)); nm_ = m_.word_count();
    }

    void init_half_prime()
    {
        lcfr::shift_right(half_prime_, prime_, size_t(1), size_t(NW));
        lcfr::add(half_prime_, half_prime_, 1, size_t(NW));
    }

public:
    /**
      Constructor taking the prime as imput and r = 4^NP / p.
      \param prime the prime number as hex string (most significant octet before)
      \param r barret reduction multiplier as hex string (most significant octet before)
    */
    pw_fp(const char* prime, const char* r)
        : prime_(prime),
          r_(r)
    {
        init_m_from_prime();
        init_half_prime();
        nr_ = r_.word_count(); // barret optimization
    }

    pw_fp(const ui<NB, W>& prime, const ui<NB, W>& r)
        : prime_(prime),
          r_(r)
    {
        init_m_from_prime();
        init_half_prime();
        nr_ = r_.word_count(); // barret optimization
    }

    /**
      \return the bit size of the prime number
    */
    size_t getPrimeBitCount() const
    {
        return NP;
    }

    /**
      \return the prime number as array of primitive integers (least significant word before)
    */
    const W* getPrime() const
    {
        return prime_;
    }

    /**
      Calculate the sum modulus prime of two integers.
      \param x the result (the array must be allocated by the client)
      \param a first addendum
      \param b second addendum
    */
    void add(W* x, const W* a, const W* b) const
    {
        auto c = lcfr::add(x, a, b, size_t(NW));
        if (FW) 
        {
            if ((c > 0) || (carry(x, m_, size_t(NW), nm_) > 0))
                lcfr::add(x, x, m_, size_t(NW), nm_);
        }
        else
        {
            if (lcfr::ge(x, prime_, size_t(NW)))
                lcfr::sub(x, x, prime_, size_t(NW));            
        }        
    }

    /**
      Calculate the difference modulus prime of two integers.
      \param x the result (the array must be allocated by the client)
      \param a minuend
      \param b subtrahend
    */
    void sub(W* x, const W* a, const W* b) const
    {
        auto c = lcfr::sub(x, a, b, size_t(NW));
        if (FW) 
        {
            if (c != 0)
                lcfr::sub(x, x, m_, size_t(NW), nm_);
        }
        else
        {
            if (c != 0)
                lcfr::add(x, x, prime_, size_t(NW));            
        }        

    }

    /**
      Calculate the product modulus prime of two integers.
      \param x the result (the array must be allocated by the client)
      \param a first operand
      \param b second operand
    */
    void mult(W* x, const W* a, const W* b) const
    {
        ui<NB * 2, W> prod_;
        ui<NB * 3, W> temp_;
        lcfr::mult(prod_, a, b, size_t(NW));
        if (FW) barret(x, prod_, m_, r_, size_t(NW), nm_, nr_, temp_);
        else    barret(x, prod_, prime_, r_, size_t(NW), size_t(NP), temp_);
    }

    /**
      Divides by two modulus prime the input number.
      \param x the result (the array must be allocated by the client)
      \param u the input number
    */
    void half(W* x, const W* u) const
    {
        bool odd = u[0] & W(1);
        lcfr::shift_right(x, u, 1, size_t(NW));
        if (odd) add(x, x, half_prime_);
    }

    /**
      Multiplies by two modulus prime the input number.
      \param x the result (the array must be allocated by the client)
      \param u the input number
    */
    void twice(W* x, const W* u) const
    {
        if (FW)
        {
            bool big = (u[NW - 1] & (W(1) << (WB - 1))) != 0;
            lcfr::shift_left(x, u, 1, size_t(NW));
            if (big || carry(x, m_, size_t(NW), nm_))
                lcfr::add(x, x, m_, NW, nm_);
        }
        else
        {
            lcfr::shift_left(x, u, 1, size_t(NW));
            if (lcfr::ge(x, prime_, size_t(NW)))
                lcfr::sub(x, x, prime_, size_t(NW));
        }
    }

    /**
      Calculates the square modulus prime of the input number.
      \param x the result (the array must be allocated by the client)
      \param a the input number
    */
    void square(W* x, const W* a) const
    {
        ui<NB * 2, W> prod_;
        ui<NB * 3, W> temp_;
        lcfr::square(prod_, a, size_t(NW));
        //::mult(prod_, a, a, size_t(NW));
        if (FW) barret(x, prod_, m_, r_, size_t(NW), nm_, nr_, temp_);
        else    barret(x, prod_, prime_, r_, size_t(NW), size_t(NP), temp_);
    }

    /**
      Calculates the inverse modulus prime of the input number (slow operation).
      \param x the result (the array must be allocated by the client)
      \param a the input number
    */
    void inverse(W* z, const W* u) const
    {
        ui<NB, W> one(W(1));
        ui<NB, W> x1(prime_);
        ui<NB, W> x2(u);

        ui<NB, W> hp; // (p + 1) / 2
        shift_right(hp, prime_, 1, NW);
        lcfr::add(hp, hp, 1, NW);

        ui<NB, W> a1(W(1));
        ui<NB, W> b1(W(0));
        ui<NB, W> a2(W(0));
        ui<NB, W> b2(W(1));

        bool swap = false;
        for (size_t i = 0; i < NB * 2; i++)
        {
            ui<NB, W>& xh(swap ? x2 : x1);
            ui<NB, W>& xl(swap ? x1 : x2);
            ui<NB, W>& ah(swap ? a2 : a1);
            ui<NB, W>& al(swap ? a1 : a2);
            ui<NB, W>& bh(swap ? b2 : b1);
            ui<NB, W>& bl(swap ? b1 : b2);

            if (eq(xl, one, NW))
            {
                lcfr::set(z, bl, NW);
                //std::copy((const W*)bl, (const W*)bl + NW, z);
                return;
            }

            bool h_odd = xh[0] & W(1);
            bool l_odd = xl[0] & W(1);

            if (h_odd && l_odd)
            {
                lcfr::sub(xh, xh, xl, NW);
                lcfr::shift_right(xh, xh, 1, NW);

                sub(ah, ah, al);
                sub(bh, bh, bl);
                half(ah, ah);
                half(bh, bh);
                if (l(xh, xl, NW)) swap = !swap;
            }
            else if (h_odd)
            {
                lcfr::shift_right(xl, xl, 1, NW);
                half(al, al);
                half(bl, bl);
            }
            else if (l_odd)
            {
                lcfr::shift_right(xh, xh, 1, NW);
                half(ah, ah);
                half(bh, bh);
                if (l(xh, xl, NW)) swap = !swap;
            }
        }
    }

    /**
      Calculates the modulus prime of the input number (slow operation).
      \param x the result (the array must be allocated by the client)
      \param a the input number, strictly less then the prime square
      \param na the input number word size
    */
    void modulo(W* x, const W* a, size_t na) const
    {
        ui<NB * 2, W> a_(a, na);
        ui<NB * 3, W> temp_;
        if (FW) barret(x, a_, m_, r_, size_t(NW), nm_, nr_, temp_);
        else    barret(x, a_, prime_, r_, size_t(NW), size_t(NP), temp_);
    }
};

}
