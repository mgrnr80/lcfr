#include "lcfr/containers/variant.h"
#include "lcfr/crypto/ecc/ec_fp.h"

namespace lcfr
{

class EcCipher
{
public:
    static const size_t MAX_COMPRESSION = 32;

    EcCipher(const char* curve);

    size_t getPrimeBitLength() const;
    size_t getPrimeByteLength() const;

    void getPrime(
        uint8_t* p, size_t p_size) const;

    uint32_t getCurvePointCoordinateBitLength() const;
    uint32_t getCurvePointCoordinateByteLength() const;

    void generatePublicKey(
        uint8_t* qx, size_t qx_size,
        uint8_t* qy, size_t qy_size,
        const uint8_t* pk, size_t pk_size) const;

    void generateSignature(
        uint8_t* r,        size_t r_size,
        uint8_t* s,        size_t s_size,
        const uint8_t* h,  size_t h_size,
        const uint8_t* ek, size_t ek_size,
        const uint8_t* pk, size_t pk_size) const;

    int32_t verifySignature(
        const uint8_t* r, size_t r_size,
        const uint8_t* s, size_t s_size,
        const uint8_t* h, size_t h_size,
        const uint8_t* qx, size_t qx_size,
        const uint8_t* qy, size_t qy_size) const;

private:
    variant<
        ec_fp_secp112r1<>,
        ec_fp_secp112r2<>,
        ec_fp_secp128r1<>,
        ec_fp_secp128r2<>,
        ec_fp_secp160k1<>,
        ec_fp_secp160r1<>,
        ec_fp_secp192k1<>,
        ec_fp_secp192r1<>,
        ec_fp_secp256k1<>,
        ec_fp_secp256r1<>
    > cipher_;
};

}
