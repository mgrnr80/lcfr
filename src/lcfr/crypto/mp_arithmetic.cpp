#include "lcfr/crypto/mp_arithmetic.h"

#ifdef X_M_X64
#include "intrin.h"
#endif

namespace lcfr {

template <class W>
W min(const W& x1, const W& x2)
{
    return x1 < x2 ? x1 : x2;
}

template <class W>
void zero_imp(W* x, size_t n)
{
    for (size_t i = 0; i < n; i++) x[i] = W(0);
}

template <class W>
void set_imp(W* x, const W* a, size_t n)
{
    for (size_t i = 0; i < n; i++) x[i] = a[i];
}

template <class W>
void set_imp(W* x, const W* a, size_t nx, size_t na)
{
    if (nx >= na)
    {
        set_imp(x, a, na);
        for (size_t i = na; i < nx; i++) x[i] = W(0);
    }
    else
    {
        set_imp(x, a, nx);
    }
}

template <class W>
W add_imp(W* x, const W* a, const W* b, size_t n)
{
    static const unsigned WB = 8 * sizeof(W);
    typedef typename uint_traits<W>::d DW;
    W c = 0;
    for (size_t i = 0; i < n; i++)
    {
        DW s = DW(a[i]) + DW(b[i]) + DW(c);
        x[i] = W(s);
        c = W(s >> WB);
    }
    return c;
}

template <class W>
W add_imp(W* x, const W* a, W c, size_t n)
{
    static const unsigned WB = 8 * sizeof(W);
    typedef typename uint_traits<W>::d DW;
    for (size_t i = 0; i < n; i++)
    {
        DW s = DW(a[i]) + DW(c);
        x[i] = W(s);
        c = W(s >> WB);
    }
    return c;
}

#ifdef X_M_X64
uint32_t add_imp(uint32_t* x, const uint32_t* a, const uint32_t* b, size_t n)
{
    static const unsigned WB = 32;
    uint8_t c = 0;
    for (size_t i = 0; i < n; i++)
    {
        c = _addcarry_u32(c, a[i], b[i], x + i);
    }
    return c;
}

uint32_t add_imp(uint32_t* x, const uint32_t* a, uint32_t c, size_t n)
{
    static const unsigned WB = 32;
    uint8_t c8 = c;
    for (size_t i = 0; i < n; i++)
    {
        c8 = _addcarry_u32(c8, a[i], 0u, x + i);
    }
    return c8;
}
#endif

template <class W>
W add_imp(W* x, const W* a, const W* b, size_t na, size_t nb)
{
    if (na >= nb)
    {
        W c = add_imp(x, a, b, nb);
        return add_imp(x + nb, a + nb, c, na - nb);
    }
    else
    {
        W c = add_imp(x, a, b, na);
        return add_imp(x + na, b + na, c, nb - na);
    }
}

template <class W>
W mult_add_imp(W* x, const W* a, const W* b, W m, size_t na, size_t nb)
{
    static const unsigned WB = 8 * sizeof(W);
    typedef typename uint_traits<W>::d DW;
    if (na > nb)
    {
        W c = 0;
        for (size_t i = 0; i < nb; i++)
        {
            DW s = DW(a[i]) + DW(b[i]) * DW(m) + DW(c); // < 2^2b - 2*2^b + 1 + 2^b - 1 + 2^b - 1 = 2^2b - 1
            x[i] = W(s);
            c = W(s >> WB);
        }
        for (size_t i = nb; i < na; i++)
        {
            DW s = DW(a[i]) + DW(c);
            x[i] = W(s);
            c = W(s >> WB);
        }
        return c;
    }
    else
    {
        W c = 0;
        for (size_t i = 0; i < na; i++)
        {
            DW s = DW(a[i]) + DW(b[i]) * DW(m) + DW(c);
            x[i] = W(s);
            c = W(s >> WB);
        }
        for (size_t i = na; i < nb; i++)
        {
            DW s = DW(b[i]) * DW(m) + DW(c); // < 2^2b - 2*2^b + 1 + 2^b - 1 + 2^b - 1 = 2^2b - 1
            x[i] = W(s);
            c = W(s >> WB);
        }
        x[nb] = c;
        return 0;
    }
}

template <class W>
typename uint_traits<W>::s sub_imp(W* x, const W* a, const W* b, size_t n)
{
    static const unsigned WB = 8 * sizeof(W);
    typedef typename uint_traits<W>::d DW;
    typedef typename uint_traits<W>::s SW;
    typedef typename uint_traits<W>::sd SDW;
    SW c = 0;
    for (size_t i = 0; i < n; i++)
    {
        DW s = DW(a[i]) - DW(b[i]) + SDW(c);
        x[i] = W(s);
        c = SW(s >> WB);
    }
    return c;
}

template <class W>
typename uint_traits<W>::s sub_imp(W* x, const W* a, typename uint_traits<W>::s c, size_t n)
{
    static const unsigned WB = 8 * sizeof(W);
    typedef typename uint_traits<W>::d DW;
    typedef typename uint_traits<W>::s SW;
    typedef typename uint_traits<W>::sd SDW;
    for (size_t i = 0; i < n; i++)
    {
        DW s = DW(a[i]) + SDW(c);
        x[i] = W(s);
        c = SW(s >> WB);
    }
    return c;
}

template <class W>
W sub_imp(W* x, const W* a, const W* b, size_t na, size_t nb)
{
    typedef typename uint_traits<W>::d DW;
    typedef typename uint_traits<W>::s SW;
    typedef typename uint_traits<W>::sd SDW;
    if (na >= nb)
    {
        auto c = sub_imp(x, a, b, nb);
        return sub_imp(x + nb, a + nb, c, na - nb);
    }
    else
    {
        auto c = sub_imp(x, a, b, na);
        static const unsigned WB = 8 * sizeof(W);
        for (size_t i = na; i < nb; i++)
        {
            DW s = SDW(c) - DW(b[i]);
            x[i] = W(s);
            c = SW(s >> WB);
        }
        return c;
    }
}

template <class W>
void mult_imp(W* x, const W* a, const W* b, size_t na, size_t nb)
{
    static const unsigned WB = 8 * sizeof(W);
    typedef typename uint_traits<W>::d DW;
    size_t nx = na + nb;
    for (size_t i = 0; i < nx; i++) x[i] = W(0);

    W c = W(0);
    for (size_t j = 0; j < nb; j++)
    {
        for (size_t i = 0; i < na; i++)
        {
            DW s = DW(x[i + j]) + DW(a[i]) * DW(b[j]) + DW(c);
            x[i + j] = W(s);
            c = W(s >> WB);
        }
        x[j + na] = c;
        c = W(0);
    }
}

template <class W>
void square_imp(W* x, const W* a, size_t n)
{
    static const unsigned WB = 8 * sizeof(W);
    typedef typename uint_traits<W>::d DW;
    size_t nx = n + n;
    for (size_t i = 0; i < nx; i++) x[i] = W(0);

    W c = W(0);
    for (size_t j = 0; j < n; j++)
    {
        for (size_t i = j + 1; i < n; i++)
        {
            DW s = DW(x[i + j]) + DW(a[i]) * DW(a[j]) + DW(c);
            x[i + j] = W(s);
            c = W(s >> WB);
        }
        x[j + n] = c;
        c = W(0);
    }

    add_imp(x, x, x, nx);

    c = W(0);
    for (size_t j = 0; j < n; j++)
    {
        DW p = DW(a[j]) * DW(a[j]);
        W pl = W(p);
        W ph = W(p >> WB);

        DW sl = DW(pl) + DW(x[2 * j]) + DW(c);
        x[2 * j] = W(sl);
        c = W(sl >> WB);

        DW sh = DW(ph) + DW(x[2 * j + 1]) + DW(c);
        x[2 * j + 1] = W(sh);
        c = W(sh >> WB);
    }
}

/*void square_imp(uint32_t* x, const uint32_t* a, size_t n)
{
    static const unsigned WB = 32;
    typedef typename uint32_t W;
    typedef typename uint64_t DW;
    size_t nx = 2 * n;
    for (size_t i = 0; i < nx; i++) x[i] = W(0);

    DW c = DW(0);
    for (size_t j = 0; j < n; j++)
    {
        W aj = a[j];
        DW p = __emulu(aj, aj);
        DW s = DW(x[2 * j]) + p;
        x[2 * j] = W(s);
        c = W(s >> WB); // <= 2*2^b - 1
        for (size_t i = j + 1; i < n; i++)
        {
            p = __emulu(a[i], aj);
            auto c8 = _addcarry_u64(0, DW(x[i + j]), p, &s);
            c8 = _addcarry_u64(c8, s, p, &s);
            c8 = _addcarry_u64(c8, s, c, &s);
            x[i + j] = W(s);
            c = W(s >> WB) + (DW(c8) << WB);
        }
        s = DW(x[j + n]) + c;
        x[j + n] = W(s);
        if (j + 1 < n) x[j + n + 1] = W(s >> WB);
        c = DW(0);
    }
}*/


/*void mult_imp(uint32_t* x, const uint32_t* a, const uint32_t* b, size_t na, size_t nb)
{
    static const unsigned WB = 32;
    size_t nx = na + nb;
    for (size_t i = 0; i < nx; i++) x[i] = 0u;

    uint32_t c = 0u;
    for (size_t j = 0; j < nb; j++)
    {
        for (size_t i = 0; i < na; i++)
        {
            auto p = __emulu(a[i], b[j]);
            auto s = p + x[i + j] + c;
            x[i + j] = uint32_t(s);
            c = uint32_t(s >> WB);
        }
        x[j + na] = c;
        c = 0u;
    }
}*/

template <class W>
typename uint_traits<W>::s two(W* x, const W* a, size_t nx, size_t na) // to be improved
{
    static const unsigned WB = 8 * sizeof(W);
    typedef typename uint_traits<W>::d DW;
    typedef typename uint_traits<W>::s SW;
    typedef typename uint_traits<W>::sd SDW;
    SW c = 0;
    for (size_t i = 0; i < na; i++)
    {
        DW s = SDW(c) - DW(a[i]);
        x[i] = W(s);
        c = SW(s >> WB);
    }
    for (size_t i = na; i < nx; i++)
    {
        DW s = SDW(c);
        x[i] = W(s);
        c = SW(s >> WB);
    }
    return c;
}

template <class W>
W carry_imp(const W* a, const W* b, size_t na, size_t nb)
{
    if (nb > na) return carry_imp(b, a, nb, na);
    static const unsigned WB = 8 * sizeof(W);
    typedef typename uint_traits<W>::d DW;
    W c = 0;
    for (size_t i = 0; i < nb; i++)
    {
        DW s = DW(a[i]) + DW(b[i]) + DW(c);
        c = W(s >> WB);
    }
    for (size_t i = nb; i < na; i++)
    {
        DW s = DW(a[i]) + DW(c);
        c = W(s >> WB);
    }
    return c;
}

template <class W>
bool z_imp(const W* a, size_t n)
{
    for (size_t i = 0; i < n; i++) if (a[i] != W(0)) return false;
    return true;
}

template <class W>
void shift_right_imp(W* x, const W* a, size_t b, size_t n)
{
    static const size_t WB = 8 * sizeof(W);
    size_t bh = min(b / WB, n);
    W bl = W(b % WB);
    W blc = WB - bl;
    auto c = (bh < n) ? a[bh] : W(0);
    for (size_t i = 1; i < n - bh; i++)
    {
        auto d = a[i + bh];
        x[i - 1] = (bl > 0) ? (c >> bl) | (d << blc) : c;
        c = d;
    }
    if (bh < n) x[n - bh - 1] = (c >> bl);
    for (size_t i = n - bh; i < n; i++) x[i] = W(0);
}

template <class W>
void shift_left_imp(W* x, size_t b, size_t n)
{
    static const size_t WB = 8 * sizeof(W);
    size_t bh = min(b / WB, n);
    W bl = W(b % WB);
    W blc = WB - bl;
    auto c = (bh < n) ? x[n - bh - 1] : W(0);
    for (size_t i = n - 1; i > bh; i--)
    {
        auto d = x[i - bh - 1];
        x[i] = (bl > 0) ? (c << bl) | (d >> blc) : c;
        c = d;
    }
    if (bh < n) x[bh] = (c << bl);
    for (size_t i = bh; i > 0; i--) x[i - 1] = W(0);
}

template <class W>
void shift_left_imp(W* x, const W* a, size_t b, size_t n)
{
    static const size_t WB = 8 * sizeof(W);
    if ((x == a) && (b >= WB)) shift_left_imp(x, b, n);
    size_t bh = min(b / WB, n);
    W bl = W(b % WB);
    W blc = WB - bl;
    zero_imp(x, bh);
    auto c = W(0);
    for (size_t i = bh; i < n; i++)
    {
        auto d = a[i - bh];
        x[i] = (blc > 0) ? (d << bl) | (c >> blc) : c;
        c = d;
    }
}

template <class W>
void xor_imp(W* x, const W* a, const W* b, size_t n)
{
    for (size_t i = 0; i < n; i++) x[i] = a[i] ^ b[i];
}

template <class W>
void and_imp(W* x, const W* a, const W* b, size_t n)
{
    for (size_t i = 0; i < n; i++) x[i] = a[i] & b[i];
}

template <class W>
void or_imp(W* x, const W* a, const W* b, size_t n)
{
    for (size_t i = 0; i < n; i++) x[i] = a[i] | b[i];
}

template <class W>
void barret_imp(W* x, const W* p, const W* m, const W* r, size_t n, size_t nm, size_t nr, W* t)
{
    // mudulus = 2^n - m
    // t = p * (2^n + r)    r ~ m (1 + m / 2^n)
    mult_imp(t, p, r, 2 * n, nr);
    add_imp(t + n, p, t + n, 2 * n, n + nr);

    // t = floor(t * 2^-2n) * (2^n - m)
    mult_imp(t, t + 2 * n, m, n, nm);
    two(t, t, 2 * n, n + nm);
    add_imp(t + n, t + 2 * n, t + n, n, n);

    // t = p - t
    sub_imp(t, p, t, 2 * n);
    bool done = z_imp(t + n, n) && (carry_imp(t, m, n, nm) == 0);
    if (done) set(x, t, n);
    else add_imp(x, t, m, n, nm);
}

template <class W>
void barret_imp(W* x, const W* prod, const W* prime, const W* r, size_t nw, size_t npb, W* t)
{
    mult_imp(t, prod, r, 2 * nw, nw);
    shift_right_imp(t, t, npb * 2, 3 * nw);

    mult_imp(t + nw, t, prime, nw, nw);
    sub_imp(t + nw, prod, t + nw, 2 * nw);

    bool done = z_imp(t + 2 * nw, nw) && l(t + nw, prime, nw);
    if (done) set(x, t + nw, nw);
    else sub_imp(x, t + nw, prime, nw);
}

template <class W>
W inverse_imp(W x) // x must be odd
{
    typedef typename uint_traits<W>::s SW;
    if (x == W(1)) return W(1);
    W q = W(-1) / x;
    W r = W(-1) % x + 1;
    W s = W(1);
    W t = -q;

    W s2 = 0;
    W s1 = s;
    W t2 = 1;
    W t1 = t;
    W r2 = x;
    W r1 = r;

    while (r1 != 0)
    {
        q = r2 / r1;
        r = r2 % r1;
        s = s2 - q * s1;
        t = t2 - q * t1;
        s2 = s1;
        s1 = s;
        t2 = t1;
        t1 = t;
        r2 = r1;
        r1 = r;
    }
    return t2;
}

template <class W>
W inverse_imp(W x, W mod) // x must be odd
{
    typedef typename uint_traits<W>::s SW;
    if (x == W(1)) return W(1);
    W q = mod / x;
    W r = mod % x;
    W s = 1;
    W t = mod - q;

    W s2 = 0;
    W s1 = s;
    W t2 = 1;
    W t1 = t;
    W r2 = x;
    W r1 = r;

    while (r1 != 0)
    {
        q = r2 / r1;
        r = r2 % r1;
        auto u1 = (q * s1) % mod;
        auto v1 = (q * t1) % mod;
        s = s2 > u1 ? s2 - u1 : mod + s2 - u1;
        t = t2 > v1 ? t2 - v1 : mod + t2 - v1;
        s2 = s1;
        s1 = s;
        t2 = t1;
        t1 = t;
        r2 = r1;
        r1 = r;
    }
    return t2;
}


template <class W>
bool eq_imp(const W* a, const W* b, size_t n)
{
    for (size_t i = 0; i < n; i++) if (a[i] != b[i]) return false;
    return true;
}

template <class W>
bool g_imp(const W* a, const W* b, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        if (a[n - i - 1] > b[n - i - 1]) return true;
        if (a[n - i - 1] < b[n - i - 1]) return false;
    }
    return false;
}

template <class W>
bool l_imp(const W* a, const W* b, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        if (a[n - i - 1] < b[n - i - 1]) return true;
        if (a[n - i - 1] > b[n - i - 1]) return false;
    }
    return false;
}

template <class W>
bool ge_imp(const W* a, const W* b, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        if (a[n - i - 1] > b[n - i - 1]) return true;
        if (a[n - i - 1] < b[n - i - 1]) return false;
    }
    return true;
}

template <class W>
bool le_imp(const W* a, const W* b, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        if (a[n - i - 1] < b[n - i - 1]) return true;
        if (a[n - i - 1] > b[n - i - 1]) return false;
    }
    return true;
}

void zero(uint32_t* x, size_t n)
{
    zero_imp(x, n);
}

void set(uint32_t* x, const uint32_t* a, size_t n)
{
    set_imp(x, a, n);
}

void set(uint32_t* x, const uint32_t* a, size_t nx, size_t na)
{
    set_imp(x, a, nx, na);
}

uint32_t add(uint32_t* x, const uint32_t* a, const uint32_t* b, size_t n)
{
    return add_imp(x, a, b, n);
}

uint32_t add(uint32_t* x, const uint32_t* a, const uint32_t* b, size_t na, size_t nb)
{
    return add_imp(x, a, b, na, nb);
}

uint32_t add(uint32_t* x, const uint32_t* a, uint32_t b, size_t n)
{
    return add_imp(x, a, b, n);
}

int32_t sub(uint32_t* x, const uint32_t* a, const uint32_t* b, size_t n)
{
    return sub_imp(x, a, b, n);
}

int32_t sub(uint32_t* x, const uint32_t* a, const uint32_t* b, size_t na, size_t nb)
{
    return sub_imp(x, a, b, na, nb);
}

void mult(uint32_t* x, const uint32_t* a, const uint32_t* b, size_t n)
{
    mult_imp(x, a, b, n, n);
}

void mult(uint32_t* x, const uint32_t* a, const uint32_t* b, size_t na, size_t nb)
{
    mult_imp(x, a, b, na, nb);
}

void square(uint32_t* x, const uint32_t* a, size_t na)
{
    //mult_imp(x, a, a, na, na);
    square_imp(x, a, na);
}

uint32_t mult_add(uint32_t* x, const uint32_t* a, const uint32_t* b, uint32_t m, size_t na, size_t nb)
{
    return mult_add_imp(x, a, b, m, na, nb);
}

bool z(const uint32_t* a, size_t n)
{
    return z_imp(a, n);
}

uint32_t carry(const uint32_t* a, const uint32_t* b, size_t na, size_t nb)
{
    return carry_imp(a, b, na, nb);
}

void shift_right(uint32_t* x, const uint32_t* a, size_t b, size_t n)
{
    shift_right_imp(x, a, b, n);
}

void shift_left(uint32_t* x, const uint32_t* a, size_t b, size_t n)
{
    shift_left_imp(x, a, b, n);
}

void shift_left(uint32_t* x, size_t b, size_t n)
{
    shift_left_imp(x, b, n);
}

void bitwise_xor(uint32_t* x, const uint32_t* a, const uint32_t* b, size_t n)
{
    xor_imp(x, a, b, n);
}

void bitwise_and(uint32_t* x, const uint32_t* a, const uint32_t* b, size_t n)
{
    and_imp(x, a, b, n);
}

void bitwise_or(uint32_t* x, const uint32_t* a, const uint32_t* b, size_t n)
{
    or_imp(x, a, b, n);
}

// p = 2^k - x
// q <= p^2 -2p +1
// z = q * (2^k + x) <= (2^k - x - 1)^2 * (2^k + x) < (2^2k - x^2) (2^k - x - 1) < 2^3k
// q - floor(z / 2^2k) * (2^k - x) = q - [q * (2^k + x) / 2^2k - y] * (2^k - x)
//   = q - q * (2^2k - x^2) / 2^2k + y * (2^k - x) < q * x^2 / 2^2k + (2^k - x) 
//   < x^2 + p
void barret(uint32_t* x, const uint32_t* p, const uint32_t* m, const uint32_t* r, size_t n, size_t nm, size_t nr, uint32_t* t)
{
    barret_imp(x, p, m, r, n, nm, nr, t);
}

void barret(uint32_t* x, const uint32_t* prod, const uint32_t* prime, const uint32_t* r, size_t nw, size_t npb, uint32_t* t)
{
    barret_imp(x, prod, prime, r, nw, npb, t);
}

uint32_t inverse(uint32_t x)
{
    return inverse_imp(x);
}

uint32_t inverse(uint32_t x, uint32_t mod)
{
    return inverse_imp(x, mod);
}

bool eq(const uint32_t* a, const uint32_t* b, size_t n)
{
    return eq_imp(a, b, n);
}

bool g(const uint32_t* a, const uint32_t* b, size_t n)
{
    return g_imp(a, b, n);
}

bool l(const uint32_t* a, const uint32_t* b, size_t n)
{
    return l_imp(a, b, n);
}

bool ge(const uint32_t* a, const uint32_t* b, size_t n)
{
    return ge_imp(a, b, n);
}

bool le(const uint32_t* a, const uint32_t* b, size_t n)
{
    return le_imp(a, b, n);
}


// 16 bit ----
void zero(uint16_t* x, size_t n)
{
    zero_imp(x, n);
}

void set(uint16_t* x, const uint16_t* a, size_t n)
{
    set_imp(x, a, n);
}

void set(uint16_t* x, const uint16_t* a, size_t nx, size_t na)
{
    set_imp(x, a, nx, na);
}

uint16_t add(uint16_t* x, const uint16_t* a, const uint16_t* b, size_t n)
{
    return add_imp(x, a, b, n);
}

uint16_t add(uint16_t* x, const uint16_t* a, const uint16_t* b, size_t na, size_t nb)
{
    return add_imp(x, a, b, na, nb);
}

uint16_t add(uint16_t* x, const uint16_t* a, uint16_t b, size_t n)
{
    return add_imp(x, a, b, n);
}

int16_t sub(uint16_t* x, const uint16_t* a, const uint16_t* b, size_t n)
{
    return sub_imp(x, a, b, n);
}

int16_t sub(uint16_t* x, const uint16_t* a, const uint16_t* b, size_t na, size_t nb)
{
    return sub_imp(x, a, b, na, nb);
}

void mult(uint16_t* x, const uint16_t* a, const uint16_t* b, size_t n)
{
    mult_imp(x, a, b, n, n);
}

void mult(uint16_t* x, const uint16_t* a, const uint16_t* b, size_t na, size_t nb)
{
    mult_imp(x, a, b, na, nb);
}

void square(uint16_t* x, const uint16_t* a, size_t na)
{
    square_imp(x, a, na);
}

uint16_t mult_add(uint16_t* x, const uint16_t* a, const uint16_t* b, uint16_t m, size_t na, size_t nb)
{
    return mult_add_imp(x, a, b, m, na, nb);
}

bool z(const uint16_t* a, size_t n)
{
    return z_imp(a, n);
}

uint16_t carry(const uint16_t* a, const uint16_t* b, size_t na, size_t nb)
{
    return carry_imp(a, b, na, nb);
}

void shift_right(uint16_t* x, const uint16_t* a, size_t b, size_t n)
{
    shift_right_imp(x, a, b, n);
}

void shift_left(uint16_t* x, const uint16_t* a, size_t b, size_t n)
{
    shift_left_imp(x, a, b, n);
}

void shift_left(uint16_t* x, size_t b, size_t n)
{
    shift_left_imp(x, b, n);
}

void bitwise_xor(uint16_t* x, const uint16_t* a, const uint16_t* b, size_t n)
{
    xor_imp(x, a, b, n);
}

void bitwise_and(uint16_t* x, const uint16_t* a, const uint16_t* b, size_t n)
{
    and_imp(x, a, b, n);
}

void bitwise_or(uint16_t* x, const uint16_t* a, const uint16_t* b, size_t n)
{
    or_imp(x, a, b, n);
}

void barret(uint16_t* x, const uint16_t* p, const uint16_t* m, const uint16_t* r, size_t n, size_t nm, size_t nr, uint16_t* t)
{
    barret_imp(x, p, m, r, n, nm, nr, t);
}

void barret(uint16_t* x, const uint16_t* prod, const uint16_t* prime, const uint16_t* r, size_t nw, size_t npb, uint16_t* t)
{
    barret_imp(x, prod, prime, r, nw, npb, t);
}

uint16_t inverse(uint16_t x)
{
    return inverse_imp(x);
}

uint16_t inverse(uint16_t x, uint16_t mod)
{
    return inverse_imp(x, mod);
}

bool eq(const uint16_t* a, const uint16_t* b, size_t n)
{
    return eq_imp(a, b, n);
}

bool g(const uint16_t* a, const uint16_t* b, size_t n)
{
    return g_imp(a, b, n);
}

bool l(const uint16_t* a, const uint16_t* b, size_t n)
{
    return l_imp(a, b, n);
}

bool ge(const uint16_t* a, const uint16_t* b, size_t n)
{
    return ge_imp(a, b, n);
}

bool le(const uint16_t* a, const uint16_t* b, size_t n)
{
    return le_imp(a, b, n);
}

}
