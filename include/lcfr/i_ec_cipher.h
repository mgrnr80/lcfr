/** \file i_ec_cipher.h
  * The file includes the lcfr library C/C++ interface.
  */
#pragma once

#include <stdint.h>

#ifdef LCFR_API
#undef LCFR_API
#endif

#ifdef __BUILD_LCFR_LIBRARY__
#ifdef _WIN32
#define LCFR_API __declspec(dllexport)
#else
#define LCFR_API
#endif
#else
#ifdef _WIN32
#define LCFR_API __declspec(dllimport)
#else
#define LCFR_API
#endif
#endif

/**
  * \struct lcfr_EcCipher_vtable_ptr_
  *
  * This struct is the C representation of an interface to a cipher
  * for generation and verification of digital signatures, using elliptic-curve cryptography.
  * Standard ECDSA as well as a proprietary compressed schema are supported.
  */
typedef struct lcfr_EcCipher_vtable_ptr_
{
    void* vtable_;
} lcfr_EcCipher_vtable_ptr;

#ifdef __cplusplus
extern "C" {
#endif

/** \brief Output the message of the last error occurred using the lcfr api, in the calling thread.
  * \param[out] _result the address of the pointer to the output string
  * \return 0 if successful, a positive number otherwise
  */
LCFR_API uint32_t lcfr_EcCipher_getExceptionMessage(char const ** _result);

/** \brief Destroy the cipher object whose interface is passed to the function.
  * \param this_ptr the address of the cipher interface
  * \return 0 if successful, a positive number otherwise
  */
LCFR_API uint32_t lcfr_EcCipher_release(lcfr_EcCipher_vtable_ptr* this_ptr);

/** \brief Create a Cipher.
  * \param[out] _result the address of the pointer to the cipher interface
  * \param curve the name of the curve
  * \return 0 if successful, a positive number otherwise
  *
  * This method is the Cipher class constructor.
  * The following are the supported curves:
  * - 56-bit - secp112r1, secp112r2
  * - 64-bit - secp128r1, secp128r2
  * - 80-bit - secp160k1, secp160r1
  * - 96-bit - secp192k1, secp192r1
  * - 128-bit - secp256k1, secp256r1
  */
LCFR_API uint32_t lcfr_EcCipher_create(
    lcfr_EcCipher_vtable_ptr** _result,
    const char* curve);

/** \brief Output the bit size of the curve points finite field prime.
  * \param this_ptr the address of the cipher interface
  * \param[out] _result the address of the output variable
  * \return 0 if successful, a positive number otherwise
  */
LCFR_API uint32_t lcfr_EcCipher_getPrimeBitLength(
    lcfr_EcCipher_vtable_ptr* this_ptr,
    uint32_t* _result);

/** \brief Output the byte size of the curve points finite field prime.
  * \param this_ptr the address of the cipher interface
  * \param[out] _result the address of the output variable
  * \return 0 if successful, a positive number otherwise
  */
LCFR_API uint32_t lcfr_EcCipher_getPrimeByteLength(
    lcfr_EcCipher_vtable_ptr* this_ptr,
    uint32_t* _result);

/** \brief Output the curve points finite field prime.
  * \param this_ptr the address of the cipher interface
  * \param[out] p the byte array to store the output prime
  * \param p_size the byte array capacity
  * \return 0 if successful, a positive number otherwise
  * \remark The output prime number is written with network byte order,
  *         if the array size exceeds required size the number is left-padded with zeros.
  */
LCFR_API uint32_t lcfr_EcCipher_getPrime(
    lcfr_EcCipher_vtable_ptr* this_ptr,
    uint8_t* p,
    uint32_t p_size);

/** \brief Return the bit size of the curve point coordinate finite field prime.
  * \param this_ptr the address of the cipher interface
  * \param[out] _result the address of the output variable
  * \return 0 if successful, a positive number otherwise
  */
LCFR_API uint32_t lcfr_EcCipher_getCurvePointCoordinateBitLength(
    lcfr_EcCipher_vtable_ptr* this_ptr,
    uint32_t* _result);

/** \brief Return the byte size of the curve point coordinate finite field prime.
  * \param this_ptr the address of the cipher interface
  * \param[out] _result the address of the output variable
  * \return 0 if successful, a positive number otherwise
  */
LCFR_API uint32_t lcfr_EcCipher_getCurvePointCoordinateByteLength(
    lcfr_EcCipher_vtable_ptr* this_ptr,
    uint32_t* _result);

/** \brief Output the cipher public key given the private key.
  * \param this_ptr the address of the cipher interface
  * \param[out] qx the byte array for the x component of the public key
  * \param qx_size the qx byte array size
  * \param[out] qy the byte array for the y component of the public key
  * \param qy_size the qy byte array size
  * \param sk the byte array storing the secret key
  * \param sk_size the sk byte array size
  * \return 0 if successful, a positive number otherwise
  * \remark All in/out numbers are written with network byte order.
  *         For each output number if the relative array size exceeds required size the number is left-padded with zeros.
  *         The secret key bit size must not exceed the bit size of the curve points finite field prime.
  */
LCFR_API uint32_t lcfr_EcCipher_generatePublicKey(
    lcfr_EcCipher_vtable_ptr* this_ptr,
    uint8_t* qx,
    uint32_t qx_size,
    uint8_t* qy,
    uint32_t qy_size,
    const uint8_t* sk,
    uint32_t sk_size);

/** \brief Generate the standard ECDSA signature.
  * \param this_ptr the address of the cipher interface
  * \param[out] r the byte array to store the r component of the signature
  * \param r_size the r byte array size
  * \param[out] s the byte array to store the s component of the signature
  * \param s_size the s byte array size
  * \param hash the byte array storing the hash
  * \param h_size the hash byte array size
  * \param ek the byte array storing the ephemeral key
  * \param ek_size the ek byte array size
  * \param sk the byte array storing the secret key
  * \param sk_size the sk byte array size
  * \return 0 if successful, a positive number otherwise
  * \remark All in/out numbers are written with network byte order.
  *         For each output number if the relative array size exceeds required size the number is left-padded with zeros.
  *         The ephemeral key and secret key bit sizes must not exceed the bit size of the curve points finite field prime.
  */
LCFR_API uint32_t lcfr_EcCipher_generateSignature(
    lcfr_EcCipher_vtable_ptr* this_ptr,
    uint8_t* r,
    uint32_t r_size,
    uint8_t* s,
    uint32_t s_size,
    const uint8_t* hash,
    uint32_t h_size,
    const uint8_t* ek,
    uint32_t ek_size,
    const uint8_t* sk,
    uint32_t sk_size);

/** \brief Verify the standard ECDSA signature.
  * \param this_ptr the address of the cipher interface
  * \param[out] _result the address of the output variable, being 0 if successful, positive number otherwise
  * \param r the byte array storing the r component of the signature
  * \param r_size the r byte array size
  * \param s the byte array storing the s component of the signature
  * \param s_size the s byte array size
  * \param hash the byte array storing the hash
  * \param h_size the hash byte array size
  * \param qx the byte array storing the x component of the public key
  * \param qx_size the qx byte array size
  * \param qy the byte array storing the y component of the public key
  * \param qy_size the qy byte array size
  * \return 0 if successful, a positive number otherwise
  * \remark All in/out numbers are written with network byte order.
  */
LCFR_API uint32_t lcfr_EcCipher_verifySignature(
    lcfr_EcCipher_vtable_ptr* this_ptr,
    int32_t* _result,
    const uint8_t* r,
    uint32_t r_size,
    const uint8_t* s,
    uint32_t s_size,
    const uint8_t* hash,
    uint32_t h_size,
    const uint8_t* qx,
    uint32_t qx_size,
    const uint8_t* qy,
    uint32_t qy_size);

#ifdef __cplusplus
}
#endif

#ifdef _WIN32
#define STDCALL __stdcall
#else
#define STDCALL
#endif

#ifdef __cplusplus
#include <string>

namespace lcfr {

/**
  * \struct IEcCipher
  *
  * This struct is the C++ interface to a cipher
  * for generation and verification of digital signatures, using elliptic-curve cryptography.
  * Standard ECDSA as well as a proprietary compressed schema are supported.
  */
struct IEcCipher
{
    /** \brief Destroy the cipher object.
      * \return 0 if successful, a positive number otherwise
      */
    virtual uint32_t STDCALL release() = 0;
    
    /** \brief Initialize the cipher.
      * \param curve the name of the curve
      *
      * This method is the Cipher class constructor.
      * The following are the supported curves:
      * - 56-bit - secp112r1, secp112r2
      * - 64-bit - secp128r1, secp128r2
      * - 80-bit - secp160k1, secp160r1
      * - 96-bit - secp192k1, secp192r1
      * - 128-bit - secp256k1, secp256r1
      */
    virtual uint32_t STDCALL init(
        const char* curve) = 0;
    
    /** \brief Output the bit size of the curve points finite field prime.
      * \param _result the address of the output variable
      */
    virtual uint32_t STDCALL getPrimeBitLength(
        uint32_t* _result) = 0;
    
    /** \brief Output the byte size of the curve points finite field prime.
      * \param _result the address of the output variable
      */
    virtual uint32_t STDCALL getPrimeByteLength(
        uint32_t* _result) = 0;
    
    /** \brief Output the curve points finite field prime.
      * \param[out] p the byte array to store the output prime
      * \param p_size the byte array capacity
      * \return 0 if successful, a positive number otherwise
      * \remark The output prime number is written with network byte order,
      *         if the array size exceeds required size the number is left-padded with zeros.
      */
    virtual uint32_t STDCALL getPrime(
        uint8_t* p,
        uint32_t p_size) = 0;
    
    /** \brief Return the bit size of the curve point coordinate finite field prime.
      * \param[out] _result the address of the output variable
      * \return 0 if successful, a positive number otherwise
      */
    virtual uint32_t STDCALL getCurvePointCoordinateBitLength(
        uint32_t* _result) = 0;
    
    /** \brief Return the byte size of the curve point coordinate finite field prime.
      * \param[out] _result the address of the output variable
      * \return 0 if successful, a positive number otherwise
      */
    virtual uint32_t STDCALL getCurvePointCoordinateByteLength(
        uint32_t* _result) = 0;
    
    /** \brief Output the cipher public key given the private key.
      * \param[out] qx the byte array for the x component of the public key
      * \param qx_size the qx byte array size
      * \param[out] qy the byte array for the y component of the public key
      * \param qy_size the qy byte array size
      * \param sk the byte array storing the secret key
      * \param sk_size the sk byte array size
      * \return 0 if successful, a positive number otherwise
      * \remark All in/out numbers are written with network byte order.
      *         For each output number if the relative array size exceeds required size the number is left-padded with zeros.
      *         The secret key bit size must not exceed the bit size of the curve points finite field prime.
      */
    virtual uint32_t STDCALL generatePublicKey(
        uint8_t* qx,
        uint32_t qx_size,
        uint8_t* qy,
        uint32_t qy_size,
        const uint8_t* sk,
        uint32_t sk_size) = 0;
    
    /** \brief Generate the standard ECDSA signature.
      * \param[out] r the byte array to store the r component of the signature
      * \param r_size the r byte array size
      * \param[out] s the byte array to store the s component of the signature
      * \param s_size the s byte array size
      * \param hash the byte array storing the hash
      * \param h_size the hash byte array size
      * \param ek the byte array storing the ephemeral key
      * \param ek_size the ek byte array size
      * \param sk the byte array storing the secret key
      * \param sk_size the sk byte array size
      * \return 0 if successful, a positive number otherwise
      * \remark All in/out numbers are written with network byte order.
      *         For each output number if the relative array size exceeds required size the number is left-padded with zeros.
      *         The ephemeral key and secret key bit sizes must not exceed the bit size of the curve points finite field prime.
      */
    virtual uint32_t STDCALL generateSignature(
        uint8_t* r,
        uint32_t r_size,
        uint8_t* s,
        uint32_t s_size,
        const uint8_t* hash,
        uint32_t h_size,
        const uint8_t* ek,
        uint32_t ek_size,
        const uint8_t* sk,
        uint32_t sk_size) = 0;
    
    /** \brief Verify the standard ECDSA signature.
      * \param[out] _result the address of the output variable, being 0 if successful, positive number otherwise
      * \param r the byte array storing the r component of the signature
      * \param r_size the r byte array size
      * \param s the byte array storing the s component of the signature
      * \param s_size the s byte array size
      * \param hash the byte array storing the hash
      * \param h_size the hash byte array size
      * \param qx the byte array storing the x component of the public key
      * \param qx_size the qx byte array size
      * \param qy the byte array storing the y component of the public key
      * \param qy_size the qy byte array size
      * \return 0 if successful, a positive number otherwise
      * \remark All in/out numbers are written with network byte order.
      */
    virtual uint32_t STDCALL verifySignature(
        int32_t* _result,
        const uint8_t* r,
        uint32_t r_size,
        const uint8_t* s,
        uint32_t s_size,
        const uint8_t* hash,
        uint32_t h_size,
        const uint8_t* qx,
        uint32_t qx_size,
        const uint8_t* qy,
        uint32_t qy_size) = 0;
};

/**
  * \class EcCipherProxy
  *
  * This class implements a cipher for generation and verification of digital signatures, using elliptic-curve cryptography.
  * Standard ECDSA as well as a proprietary compressed schema are supported.
  */
class EcCipherProxy
{
    lcfr::IEcCipher* obj_;
    
    public:
    
    /** \brief Create an EcCipherProxy.
      * \param curve the name of the curve
      *
      * This method is the Cipher class constructor.
      * The following are the supported curves:
      * - 56-bit - secp112r1, secp112r2
      * - 64-bit - secp128r1, secp128r2
      * - 80-bit - secp160k1, secp160r1
      * - 96-bit - secp192k1, secp192r1
      * - 128-bit - secp256k1, secp256r1
      */
    EcCipherProxy(
        const std::string& curve)
    {
        int code = lcfr_EcCipher_create(
            (lcfr_EcCipher_vtable_ptr**)&obj_,
            curve.c_str());
        if (code != 0)
        {
            const char* message;
            lcfr_EcCipher_getExceptionMessage(&message);
            throw new std::runtime_error(message);
        }
    }
    
    /** \brief Return the bit size of the curve points finite field prime.
      */
    uint32_t getPrimeBitLength()
    {
        uint32_t _result;
        int code = obj_->getPrimeBitLength(
            &_result);
        if (code != 0)
        {
            const char* message;
            lcfr_EcCipher_getExceptionMessage(&message);
            throw new std::runtime_error(message);
        }
        return _result;
    }
    
    /** \brief Return the byte size of the curve points finite field prime.
      */
    uint32_t getPrimeByteLength()
    {
        uint32_t _result;
        int code = obj_->getPrimeByteLength(
            &_result);
        if (code != 0)
        {
            const char* message;
            lcfr_EcCipher_getExceptionMessage(&message);
            throw new std::runtime_error(message);
        }
        return _result;
    }
    
    /** \brief Output the curve points finite field prime.
      * \param[out] p the byte array to store the output prime
      * \param p_size the byte array capacity
      * \remark The output prime number is written with network byte order,
      *         if the array size exceeds required size the number is left-padded with zeros.
      */
    void getPrime(
        uint8_t* p,
        uint32_t p_size)
    {
        int code = obj_->getPrime(
            p,
            p_size);
        if (code != 0)
        {
            const char* message;
            lcfr_EcCipher_getExceptionMessage(&message);
            throw new std::runtime_error(message);
        }
    }
    
    /** \brief Return the bit size of the curve point coordinate finite field prime.
      */
    uint32_t getCurvePointCoordinateBitLength()
    {
        uint32_t _result;
        int code = obj_->getCurvePointCoordinateBitLength(
            &_result);
        if (code != 0)
        {
            const char* message;
            lcfr_EcCipher_getExceptionMessage(&message);
            throw new std::runtime_error(message);
        }
        return _result;
    }
    
    /** \brief Return the byte size of the curve point coordinate finite field prime.
      */
    uint32_t getCurvePointCoordinateByteLength()
    {
        uint32_t _result;
        int code = obj_->getCurvePointCoordinateByteLength(
            &_result);
        if (code != 0)
        {
            const char* message;
            lcfr_EcCipher_getExceptionMessage(&message);
            throw new std::runtime_error(message);
        }
        return _result;
    }
    
    /** \brief Output the cipher public key given the private key.
      * \param[out] qx the byte array for the x component of the public key
      * \param qx_size the qx byte array size
      * \param[out] qy the byte array for the y component of the public key
      * \param qy_size the qy byte array size
      * \param sk the byte array storing the secret key
      * \param sk_size the sk byte array size
      * \remark All in/out numbers are written with network byte order.
      *         For each output number if the relative array size exceeds required size the number is left-padded with zeros.
      *         The secret key bit size must not exceed the bit size of the curve points finite field prime.
      */
    void generatePublicKey(
        uint8_t* qx,
        uint32_t qx_size,
        uint8_t* qy,
        uint32_t qy_size,
        const uint8_t* sk,
        uint32_t sk_size)
    {
        int code = obj_->generatePublicKey(
            qx,
            qx_size,
            qy,
            qy_size,
            sk,
            sk_size);
        if (code != 0)
        {
            const char* message;
            lcfr_EcCipher_getExceptionMessage(&message);
            throw new std::runtime_error(message);
        }
    }
    
    /** \brief Generate the standard ECDSA signature.
      * \param[out] r the byte array to store the r component of the signature
      * \param r_size the r byte array size
      * \param[out] s the byte array to store the s component of the signature
      * \param s_size the s byte array size
      * \param hash the byte array storing the hash
      * \param h_size the hash byte array size
      * \param ek the byte array storing the ephemeral key
      * \param ek_size the ek byte array size
      * \param sk the byte array storing the secret key
      * \param sk_size the sk byte array size
      * \remark All in/out numbers are written with network byte order.
      *         For each output number if the relative array size exceeds required size the number is left-padded with zeros.
      *         The ephemeral key and secret key bit sizes must not exceed the bit size of the curve points finite field prime.
      */
    void generateSignature(
        uint8_t* r,
        uint32_t r_size,
        uint8_t* s,
        uint32_t s_size,
        const uint8_t* hash,
        uint32_t h_size,
        const uint8_t* ek,
        uint32_t ek_size,
        const uint8_t* sk,
        uint32_t sk_size)
    {
        int code = obj_->generateSignature(
            r,
            r_size,
            s,
            s_size,
            hash,
            h_size,
            ek,
            ek_size,
            sk,
            sk_size);
        if (code != 0)
        {
            const char* message;
            lcfr_EcCipher_getExceptionMessage(&message);
            throw new std::runtime_error(message);
        }
    }
    
    /** \brief Verify the standard ECDSA signature.
      * \param r the byte array storing the r component of the signature
      * \param r_size the r byte array size
      * \param s the byte array storing the s component of the signature
      * \param s_size the s byte array size
      * \param hash the byte array storing the hash
      * \param h_size the hash byte array size
      * \param qx the byte array storing the x component of the public key
      * \param qx_size the qx byte array size
      * \param qy the byte array storing the y component of the public key
      * \param qy_size the qy byte array size
      * \return -1 if the signature is valid, 0 otherwise
      * \remark All in/out numbers are written with network byte order.
      */
    int32_t verifySignature(
        const uint8_t* r,
        uint32_t r_size,
        const uint8_t* s,
        uint32_t s_size,
        const uint8_t* hash,
        uint32_t h_size,
        const uint8_t* qx,
        uint32_t qx_size,
        const uint8_t* qy,
        uint32_t qy_size)
    {
        int32_t _result;
        int code = obj_->verifySignature(
            &_result,
            r,
            r_size,
            s,
            s_size,
            hash,
            h_size,
            qx,
            qx_size,
            qy,
            qy_size);
        if (code != 0)
        {
            const char* message;
            lcfr_EcCipher_getExceptionMessage(&message);
            throw new std::runtime_error(message);
        }
        return _result;
    }
        
    ~EcCipherProxy()
    {
        obj_->release();
    }
};

#ifndef __BUILD_LCFR_LIBRARY__
typedef EcCipherProxy EcCipher;
#endif
}
#endif

