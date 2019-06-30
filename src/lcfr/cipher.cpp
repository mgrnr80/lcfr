#include <string.h>
#include "lcfr/crypto/mp_arithmetic.h"
#include "cipher.h"

namespace lcfr {

EcCipher::EcCipher(const char* curve)
{
    if      (strcmp(curve, "secp112r1") == 0) cipher_.emplace<ec_fp_secp112r1<>>();
    else if (strcmp(curve, "secp112r2") == 0) cipher_.emplace<ec_fp_secp112r2<>>();
    else if (strcmp(curve, "secp128r1") == 0) cipher_.emplace<ec_fp_secp128r1<>>();
    else if (strcmp(curve, "secp128r2") == 0) cipher_.emplace<ec_fp_secp128r2<>>();
    else if (strcmp(curve, "secp160k1") == 0) cipher_.emplace<ec_fp_secp160k1<>>();
    else if (strcmp(curve, "secp160r1") == 0) cipher_.emplace<ec_fp_secp160r1<>>();
    else if (strcmp(curve, "secp192k1") == 0) cipher_.emplace<ec_fp_secp192k1<>>();
    else if (strcmp(curve, "secp192r1") == 0) cipher_.emplace<ec_fp_secp192r1<>>();
    else if (strcmp(curve, "secp256k1") == 0) cipher_.emplace<ec_fp_secp256k1<>>();
    else if (strcmp(curve, "secp256r1") == 0) cipher_.emplace<ec_fp_secp256r1<>>();
    else throw std::runtime_error("invalid curve name");
}

size_t EcCipher::getPrimeBitLength() const
{
    size_t bitCount = cipher_.as<ec_cipher_base<>>().get_prime_bit_length();
    return bitCount;
}

size_t EcCipher::getPrimeByteLength() const
{
    size_t bitCount = cipher_.as<ec_cipher_base<>>().get_prime_bit_length();
    return (bitCount + 7) / 8;
}

void EcCipher::getPrime(
    uint8_t* p, size_t p_size) const
{
    cipher_.as<ec_cipher_base<>>().get_prime(p, p_size);
}

uint32_t EcCipher::getCurvePointCoordinateBitLength() const
{
    size_t bitCount = cipher_.as<ec_cipher_base<>>().get_curve_point_coordinate_bit_length();
    return bitCount;
}

uint32_t EcCipher::getCurvePointCoordinateByteLength() const
{
    size_t bitCount = cipher_.as<ec_cipher_base<>>().get_curve_point_coordinate_bit_length();
    return (bitCount + 7) / 8;
}

void EcCipher::generateSignature(
    uint8_t* r,        size_t r_size,
    uint8_t* s,        size_t s_size,
    const uint8_t* h,  size_t h_size,
    const uint8_t* ek, size_t ek_size,
    const uint8_t* pk, size_t pk_size) const
{
    cipher_.as<ec_cipher_base<>>().generate_signature(
        r, r_size, s, s_size, h, h_size, ek, ek_size, pk, pk_size);
}

int32_t EcCipher::verifySignature(
    const uint8_t* r, size_t r_size,
    const uint8_t* s, size_t s_size,
    const uint8_t* h, size_t h_size,
    const uint8_t* qx, size_t qx_size,
    const uint8_t* qy, size_t qy_size) const
{
    return cipher_.as<ec_cipher_base<>>().verify_signature(
        r, r_size, s, s_size, h, h_size, qx, qx_size, qy, qy_size) ? -1 : 0; 
}

void EcCipher::generatePublicKey(
    uint8_t* qx, size_t qx_size,
    uint8_t* qy, size_t qy_size,
    const uint8_t* pk, size_t pk_size) const
{
    return cipher_.as<ec_cipher_base<>>().generate_public_key(
        qx, qx_size, qy, qy_size, pk, pk_size);
}

}
