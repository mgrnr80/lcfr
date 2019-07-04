#pragma once

#include "lcfr/arch/endianness.h"
#include "lcfr/crypto/fp.h"
#include "lcfr/crypto/ecc/ec_point.h"

namespace lcfr {

template <class W = uint32_t>
class ec_cipher_base
{
public:

    virtual size_t get_prime_bit_length() const = 0;
    virtual size_t get_prime_byte_length() const = 0;

    virtual void get_prime(
        uint8_t* p, size_t p_size) const = 0;

    virtual size_t get_curve_point_coordinate_bit_length() const = 0;
    virtual size_t get_curve_point_coordinate_byte_length() const = 0;

    virtual void generate_public_key(
        uint8_t* qx, size_t qx_size,
        uint8_t* qy, size_t qy_size,
        const uint8_t* pk, size_t pk_size) const = 0;

    virtual void generate_signature(
        uint8_t* r, size_t r_size,
        uint8_t* s, size_t s_size,
        const uint8_t* h, size_t h_size,
        const uint8_t* ek, size_t ek_size,
        const uint8_t* pk, size_t pk_size) const = 0;

    virtual bool verify_signature(
        const uint8_t* r, size_t r_size,
        const uint8_t* s, size_t s_size,
        const uint8_t* h, size_t h_size,
        const uint8_t* qx, size_t qx_size,
        const uint8_t* qy, size_t qy_size) const = 0;

    virtual const W* get_prime() const = 0;
    virtual bool sign(W* r, W* s, const W* hash, const W* ek, const W* pk) const = 0;
    virtual bool verify(const W* r, const W* s, const W* hash, const W* qx, const W* qy) const = 0;
};

template <unsigned NPB, unsigned NNB, class W = uint32_t>
class ec_cipher: public ec_cipher_base<W>
{
public:
    static const unsigned WO = sizeof(W);
    static const unsigned WB = 8 * WO;
    static const unsigned NPW = (NPB + WB - 1) / WB;
    static const unsigned NPO = (NPB + 7) / 8;
    static const unsigned NNW = (NNB + WB - 1) / WB;
    static const unsigned NNO = (NNB + 7) / 8;
    static const unsigned SEC = NNB / 2;
    static const unsigned NNB_ = NNB;

    typedef ui<NPW * WB, W>         p_ui;
    typedef ui<NNW * WB, W>         n_ui;
    typedef ui<NNW * WB * 2, W>     z_ui;

public:
    typedef typename uint_traits<W>::s SW;
    typedef ec_point<NPW * WB, W>   ecp;
    typedef ec_point_p<NPW * WB, W> ecpp;

    p_ui A;     // Curve equation parameter
    p_ui B;     // Curve equation parameter
    ecp  G;     // Base point (normalized)

    const pw_fp<NPB, W>    p_fp_;
    const pw_fp<NNB, W>    n_fp_;

public:
    
    ec_cipher(const p_ui& a, const p_ui& b,
        const p_ui& gx, const p_ui& gy,
        const p_ui& p, const p_ui& pr,
        const n_ui& n, const n_ui& nr)
        : A(a),
          B(b),
          G(gx, gy),
          p_fp_(p, pr),
          n_fp_(n, nr)
    {
    }

    virtual void get_prime(
        uint8_t* p, size_t p_size) const
    {
        n_ui p_box(get_prime());
        p_box.to_bytes(p, p_size);
    }

    virtual size_t get_prime_bit_length() const
    {
        return n_fp_.getPrimeBitCount();
    }

    virtual size_t get_prime_byte_length() const
    {
        return (n_fp_.getPrimeBitCount() + 7) / 8;
    }

    virtual size_t get_curve_point_coordinate_bit_length() const
    {
        return p_fp_.getPrimeBitCount();
    }

    virtual size_t get_curve_point_coordinate_byte_length() const
    {
        return (p_fp_.getPrimeBitCount() + 7) / 8;
    }

    virtual void generate_signature(
        uint8_t* r, size_t r_size,
        uint8_t* s, size_t s_size,
        const uint8_t* h, size_t h_size,
        const uint8_t* ek, size_t ek_size,
        const uint8_t* pk, size_t pk_size) const
    {
        n_ui r_box, s_box;

        n_ui mask = n_ui::ones(n_fp_.getPrimeBitCount());
        n_ui ek_box(ek, ek_size); bitwise_and(ek_box, ek_box, mask, NNW);
        n_ui pk_box(pk, pk_size); bitwise_and(pk_box, pk_box, mask, NNW);

        n_ui h_box; box_hash(h_box, h, h_size);

        sign(r_box, s_box, h_box, ek_box, pk_box);

        r_box.to_bytes(r, r_size);
        s_box.to_bytes(s, s_size);
    }

    virtual bool verify_signature(
        const uint8_t* r, size_t r_size,
        const uint8_t* s, size_t s_size,
        const uint8_t* h, size_t h_size,
        const uint8_t* qx, size_t qx_size,
        const uint8_t* qy, size_t qy_size) const
    {
        n_ui r_box(r, r_size);
        n_ui s_box(s, s_size);
        n_ui qx_box(qx, qx_size);
        n_ui qy_box(qy, qy_size);

        n_ui h_box; box_hash(h_box, h, h_size);

        return verify(r_box, s_box, h_box, qx_box, qy_box);
    }

    virtual void generate_public_key(
        uint8_t* qx, size_t qx_size,
        uint8_t* qy, size_t qy_size,
        const uint8_t* pk, size_t pk_size) const
    {
        p_ui qx_box, qy_box;
        n_ui mask = n_ui::ones(n_fp_.getPrimeBitCount());
        n_ui pk_box(pk, pk_size); bitwise_and(pk_box, pk_box, mask, NNW);
        public_key(qx_box, qy_box, pk_box);
        qx_box.to_bytes(qx, qx_size);
        qy_box.to_bytes(qy, qy_size);
    }

    void twice(ecp& s, const ecp& p) const
    {
        if (p.is_zero() || (p.y == p_ui::ZERO))
        {
            s = ecp();
            return;
        }
        p_ui xq, xq3, u, v, iv, lmb, t, w; // can be reduced
        p_fp_.square(xq, p.x);        // x^2
        p_fp_.mult(xq3, p_ui(W(3)), xq); // 3x^2 CAN be optimized since 3 is small
        p_fp_.add(u, xq3, A);         // u = 3x^2 + A
        p_fp_.twice(v, p.y);          // v = 2y
        p_fp_.inverse(iv, v);
        p_fp_.mult(lmb, u, iv);
        p_fp_.square(t, lmb);
        p_fp_.sub(t, t, p.x);
        p_fp_.sub(t, t, p.x);
        p_fp_.sub(w, p.x, t);
        p_fp_.mult(w, lmb, w);
        p_fp_.sub(s.y, w, p.y);
        s.x = t;
    }

    void twice(ecpp& s, const ecpp& p) const
    {
        if (p.is_zero() || (p.y == p_ui::ZERO))
        {
            s = ecpp();
            return;
        }

        p_ui xq, zq, azq, u, v, xq3, uq, vy, w, t; // can be reduced
        p_fp_.square(xq, p.x);           // x^2
        p_fp_.square(zq, p.z);           // z^2
        p_fp_.twice(xq3, xq);            // 2x^2
        p_fp_.add(xq3, xq3, xq);         // 3x^2
        p_fp_.mult(azq, A, zq);          // CAN be optimized if A is small
        p_fp_.add(u, xq3, azq);          // u = 3x^2 + Az^2
        p_fp_.mult(v, p.y, p.z);         // yz
        p_fp_.twice(v, v);               // v = 2yz
        p_fp_.square(uq, u);             // u^2
        p_fp_.mult(vy, v, p.y);          // vy
        p_fp_.mult(t, p.x, vy);          // vxy
        p_fp_.twice(t, t);               // 2vxy
        p_fp_.twice(w, t);               // 4vxy
        p_fp_.sub(w, uq, w);             // w = u^2 - 4vxy
        p_fp_.sub(xq, t, w);             // 2vxy - w
        p_fp_.mult(xq, xq, u);           // u(2vxy - w)
        p_fp_.square(zq, vy);            // v^2y^2
        p_fp_.twice(zq, zq);             // 2v^2y^2
        p_fp_.sub(s.y, xq, zq);          // y'
        p_fp_.mult(s.x, w, v);           // x'
        p_fp_.square(s.z, v);            // v^2
        p_fp_.mult(s.z, s.z, v);         // z'
    }

    void add(ecp& s, const ecp& p1, const ecp& p2) const
    {
        if (p1.is_zero())
        {
            s = p2; return;
        }
        if (p2.is_zero())
        {
            s = p1; return;
        }

        p_ui u, v; // can be reduced
        p_fp_.sub(u, p2.y, p1.y);
        p_fp_.sub(v, p2.x, p1.x);
        
        if (v == p_ui::ZERO)
        {
            if (u == p_ui::ZERO) twice(s, p1);
            else s = ecp();
            return;
        }

        p_ui iv, lmb, t, w;
        p_fp_.inverse(iv, v);
        p_fp_.mult(lmb, u, iv);
        p_fp_.square(t, lmb);
        p_fp_.sub(t, t, p1.x);
        p_fp_.sub(t, t, p2.x);
        p_fp_.sub(w, p1.x, t);
        p_fp_.mult(w, lmb, w);
        p_fp_.sub(s.y, w, p1.y);
        s.x = t;
    }

    void add(ecpp& s, const ecpp& p1, const ecpp& p2) const
    {
        if (p1.is_zero())
        {
            s = p2; return;
        }
        if (p2.is_zero())
        {
            s = p1; return;
        }

        p_ui u0, u1, v0, v1, u, v;

        p_fp_.mult(u0, p2.y, p1.z);
        p_fp_.mult(u1, p1.y, p2.z);
        p_fp_.mult(v0, p2.x, p1.z);
        p_fp_.mult(v1, p1.x, p2.z);
        p_fp_.sub(u, u0, u1);
        p_fp_.sub(v, v0, v1);

        if (v == p_ui::ZERO)
        {
            if (u == p_ui::ZERO) twice(s, p1);
            else s = ecpp();
            return;
        }

        p_ui z1z2, vq, vqz2, vc, v3z2, uq, w, w2, t, vcy1z2;
        p_fp_.mult(z1z2, p1.z, p2.z);
        p_fp_.square(vq, v);
        p_fp_.mult(vqz2, vq, p2.z);
        p_fp_.square(uq, u);
        p_fp_.add(w2, v0, v1);
        p_fp_.mult(w2, vq, w2);
        p_fp_.mult(w, uq, z1z2);
        p_fp_.sub(w, w, w2); // w = x/v
        p_fp_.mult(vcy1z2, vqz2, v);
        p_fp_.mult(vcy1z2, vcy1z2, p1.y);
        p_fp_.mult(s.y, vqz2, p1.x);
        p_fp_.sub(s.y, s.y, w);
        p_fp_.mult(s.y, s.y, u);
        p_fp_.sub(s.y, s.y, vcy1z2);
        p_fp_.mult(s.x, w, v);
        p_fp_.mult(s.z, vqz2, p1.z);
        p_fp_.mult(s.z, s.z, v);
    }

    void mult(ecp& p, const ecp& b, const W* k, size_t nk) const
    {
        ecp b2n(b);
        p = ecp();
        for (size_t i = 0; i < nk; i++)
        {
            for (size_t j = 0; j < WB; j++)
            {
                bool take = k[i] & (W(1) << j);
                if (take) add(p, p, b2n);
                twice(b2n, b2n);
            }
        }
    }

    void mult(ecpp& p, const ecpp& b, const W* k, size_t nk) const
    {
        ecpp b2n(b);
        p = ecpp();
        for (size_t i = 0; i < nk; i++)
        {
            for (size_t j = 0; j < WB; j++)
            {
                bool take = k[i] & (W(1) << j);
                if (take) add(p, p, b2n);
                twice(b2n, b2n);
            }
        }
    }

    virtual const W* get_prime() const
    {
        return n_fp_.getPrime();
    }

    virtual bool sign(W* r, W* s, const W* hash, const W* ek, const W* pk) const
    {
        //if (::eq(ek, n_ui::ZERO, NNW) || lcfr::ge(ek, n_fp_.getPrime(), NNW)) return false;

        n_ui ek_; set_modulo(ek_, ek);
        n_ui pk_; set_modulo(pk_, pk);

        ecpp p(G.x, G.y);
        mult(p, p, ek, NNW);
        normalize(p);

        n_ui r_; n_fp_.modulo(r_, p.x, NPW);
        if (r_ == n_ui::ZERO) return false;

        n_ui s_(r_);
        n_ui z_; set_modulo(z_, hash);

        n_fp_.mult(s_, s_, pk_);
        n_fp_.add(s_, s_, z_);

        n_ui ek_inv;
        n_fp_.inverse(ek_inv, ek_);
        n_fp_.mult(s_, s_, ek_inv);
        if (s_ == n_ui::ZERO) return false;

        n_ui ns(W(0));
        n_fp_.sub(ns, ns, s_);
        if (l(ns, s_, NNW)) lcfr::set(s, ns, NNW);
        else                lcfr::set(s, s_, NNW);
        lcfr::set(r, r_, NNW);
        return true;
    }

    virtual bool verify(const W* r, const W* s, const W* hash, const W* qx, const W* qy) const
    {
        n_ui r_(r), s_(s);
        n_ui z_; set_modulo(z_, hash);
        n_ui w_; n_fp_.inverse(w_, s_);

        n_ui u1_; n_fp_.mult(u1_, z_, w_);
        n_ui u2_; n_fp_.mult(u2_, r_, w_);

        ecpp p1(G.x, G.y); mult(p1, p1, u1_, NNW);
        ecpp p2(qx, qy);   mult(p2, p2, u2_, NNW);
        ecpp p;            add(p, p1, p2);
        normalize(p);

        n_ui rt_; n_fp_.modulo(rt_, p.x, NPW);
        return lcfr::eq(rt_, r_, NNW);
    }

    void public_key(W* qx, W* qy, const W* pk) const
    {
        ecpp p(G.x, G.y);
        mult(p, p, pk, NNW);
        normalize(p);

        lcfr::set(qx, p.x, NPW);
        lcfr::set(qy, p.y, NPW);
    }

    void normalize(ecpp& p) const
    {
        p_ui iz;
        p_fp_.inverse(iz, p.z);
        p_fp_.mult(p.x, p.x, iz);
        p_fp_.mult(p.y, p.y, iz);
        p.z = p_ui::ONE;
    }


    void box_hash(n_ui& h_box, const uint8_t* hash, size_t hash_len) const
    {
        hash_len = get_prime_byte_length() < hash_len ? get_prime_byte_length() : hash_len;
        h_box = n_ui(hash, hash_len);
        if (hash_len * 8 > get_prime_bit_length())
            shift_right(h_box, h_box, hash_len * 8 - get_prime_bit_length(), NNW);
    }

    void set_modulo(n_ui& y, const W* x) const
    {
        if (ge(x, n_fp_.getPrime(), NNW)) sub(y, x, n_fp_.getPrime(), NNW);
        else                              set(y, x, NNW);
    }
};


template <class W = uint32_t>
class ec_fp_secp256k1 : public ec_cipher<256, 256, W>
{
public:
    ec_fp_secp256k1()
        : ec_cipher<256, 256, W>(
            W(0), W(7), 
            "79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798",
            "483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8",
            "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F", "1000003D1",
            "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141", "14551231950B75FC4402DA1732FC9BEC0")
    {}
};

template <class W = uint32_t>
class ec_fp_secp256r1 : public ec_cipher<256, 256, W>
{
public:
    ec_fp_secp256r1()
        : ec_cipher<256, 256, W>(
            "FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFC",
            "5AC635D8AA3A93E7B3EBBD55769886BC651D06B0CC53B0F63BCE3C3E27D2604B",
            "6B17D1F2E12C4247F8BCE6E563A440F277037D812DEB33A0F4A13945D898C296",
            "4FE342E2FE1A7F9B8EE7EB4A7C0F9E162BCE33576B315ECECBB6406837BF51F5",
            "FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF", "FFFFFFFFFFFFFFFEFFFFFFFEFFFFFFFEFFFFFFFF0000000000000003",
            "FFFFFFFF00000000FFFFFFFFFFFFFFFFBCE6FAADA7179E84F3B9CAC2FC632551", "FFFFFFFFFFFFFFFEFFFFFFFF43190552DF1A6C21012FFD85EEDF9BFE")
    {}
};

template <class W = uint32_t>
class ec_fp_secp192k1 : public ec_cipher<192, 192, W>
{
public:
    ec_fp_secp192k1()
        : ec_cipher<192, 192, W>(
            W(0), W(3),
            "DB4FF10EC057E9AE26B07D0280B7F4341DA5D1B1EAE06C7D",
            "9B2F2F6D9C5628A7844163D015BE86344082AA88D95E2F9D",
            "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFEE37", "1000011C9",
            "FFFFFFFFFFFFFFFFFFFFFFFE26F2FC170F69466A74DEFD8D", "1D90D03E8F096B9958B210276")
    {}
};

template <class W = uint32_t>
class ec_fp_secp192r1 : public ec_cipher<192, 192, W>
{
public:
    ec_fp_secp192r1()
        : ec_cipher<192, 192, W>(
            "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFC",
            "64210519E59C80E70FA7E9AB72243049FEB8DEECC146B9B1",
            "188DA80EB03090F67CBF20EB43A18800F4FF0AFD82FF1012",
            "07192B95FFC8DA78631011ED6B24CDD573F977A11E794811",
            "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFF", "10000000000000001",
            "FFFFFFFFFFFFFFFFFFFFFFFF99DEF836146BC9B1B4D22831", "662107C9EB94364E4B2DD7CF")
    {}
};

template <class W = uint32_t>
class ec_fp_secp160k1 : public ec_cipher<160, 161, W>
{
public:
    ec_fp_secp160k1()
        : ec_cipher<160, 161, W>(
            W(0), W(7),
            "3B4C382CE37AA192A4019E763036F4F5DD4D7EBB",
            "938CF935318FDCED6BC28286531733C3F03C4FEE",
            "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFAC73", "10000538D",
            "100000000000000000001B8FA16DFAB9ACA16B6B3", "3FFFFFFFFFFFFFFFFFFF91C17A4815194D7A5253F")
    {}
};

template <class W = uint32_t>
class ec_fp_secp160r1 : public ec_cipher<160, 161, W>
{
public:
    ec_fp_secp160r1()
        : ec_cipher<160, 161, W>(
            "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7FFFFFFC",
            "1C97BEFC54BD7A8B65ACF89F81D4D4ADC565FA45",
            "4A96B5688EF573284664698968C38BB913CBFC82",
            "23A628553168947D59DCC912042351377AC5FB32",
            "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7FFFFFFF", "80000001",
            "100000000000000000001F4C8F927AED3CA752257", "3FFFFFFFFFFFFFFFFFFF82CDC1B6144B0D62B76B3")
    {}
};

template <class W = uint32_t>
class ec_fp_secp128r1 : public ec_cipher<128, 128, W>
{
public:
    ec_fp_secp128r1()
        : ec_cipher<128, 128, W>(
            "FFFFFFFDFFFFFFFFFFFFFFFFFFFFFFFC",
            "E87579C11079F43DD824993C2CEE5ED3",
            "161FF7528B899B2D0C28607CA52C5B86",
            "CF5AC8395BAFEB13C02DA292DDED7A83",
            "FFFFFFFDFFFFFFFFFFFFFFFFFFFFFFFF", "2000000040000000800000011",
            "FFFFFFFE0000000075A30D1B9038A115", "2000000038A5CF2EA993B2A87")
    {}
};

template <class W = uint32_t>
class ec_fp_secp128r2 : public ec_cipher<128, 126, W>
{
public:
    ec_fp_secp128r2()
        : ec_cipher<128, 126, W>(
            "D6031998D1B3BBFEBF59CC9BBFF9AEE1",
            "5EEEFCA380D02919DC2C6558BB6D8A5D",
            "7B6AA5D85E572983E6FB32A7CDEBC140",
            "27B6916A894D3AEE7106FE805FC34B44",
            "FFFFFFFDFFFFFFFFFFFFFFFFFFFFFFFF", "2000000040000000800000011",
            "3FFFFFFF7FFFFFFFBE0024720613B5A3", "400000008000000141FFDB9101EBB89C")
    {}
};

template <class W = uint32_t>
class ec_fp_secp112r1 : public ec_cipher<112, 112, W>
{
public:
    ec_fp_secp112r1()
        : ec_cipher<112, 112, W>(
            "DB7C2ABF62E35E668076BEAD2088",
            "659EF8BA043916EEDE8911702B22",
            "09487239995A5EE76B55F9C2F098",
            "A89CE5AF8724C0A23E0E0FF77500",
            "DB7C2ABF62E35E668076BEAD208B", "12A97000000000000000000000000",
            "DB7C2ABF62E35E7628DFAC6561C5", "12A96FFFFFFFFFFEAB2EA46B3447E")
    {}
};

template <class W = uint32_t>
class ec_fp_secp112r2 : public ec_cipher<112, 110, W>
{
public:
    ec_fp_secp112r2()
        : ec_cipher<112, 110, W>(
            "6127C24C05F38A0AAAF65C0EF02C",
            "51DEF1815DB5ED74FCC34C85D709",
            "4BA30AB5E892B4E1649DD0928643",
            "ADCD46F5882E3747DEF36E956E97",
            "DB7C2ABF62E35E668076BEAD208B", "12A97000000000000000000000000",
            "36DF0AAFD8B8D7597CA10520D04B", "4AA5C0000000005741402575BCFC")
    {}
};

}
