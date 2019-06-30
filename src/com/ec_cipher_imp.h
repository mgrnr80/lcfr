#pragma once

#include <stdint.h>
#include <memory>
#include <string>
#include <exception>
#include <atomic>
#include "lcfr/cipher.h"
#include "lcfr/i_ec_cipher.h"

namespace lcfr {

struct EcCipherImp : public IEcCipher
{
    static thread_local std::string exceptionMessage_;
    std::unique_ptr<EcCipher> object_;
    
    EcCipherImp();
    EcCipherImp(std::unique_ptr<EcCipher>&& obj);
    
    virtual uint32_t STDCALL release();
    
    void* STDCALL getObject();
    
    virtual uint32_t STDCALL init(
        const char* curve);
    
    virtual uint32_t STDCALL getPrimeBitLength(
        uint32_t* _result);
    
    virtual uint32_t STDCALL getPrimeByteLength(
        uint32_t* _result);
    
    virtual uint32_t STDCALL getPrime(
        uint8_t* p,
        uint32_t p_size);
    
    virtual uint32_t STDCALL getCurvePointCoordinateBitLength(
        uint32_t* _result);
    
    virtual uint32_t STDCALL getCurvePointCoordinateByteLength(
        uint32_t* _result);
    
    virtual uint32_t STDCALL generatePublicKey(
        uint8_t* qx,
        uint32_t qx_size,
        uint8_t* qy,
        uint32_t qy_size,
        const uint8_t* sk,
        uint32_t sk_size);
    
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
        uint32_t sk_size);
    
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
        uint32_t qy_size);  
};

}
