#include <memory.h>
#include "com/ec_cipher_imp.h"

namespace lcfr {

thread_local std::string EcCipherImp::exceptionMessage_;

EcCipherImp::EcCipherImp()
{}

EcCipherImp::EcCipherImp(std::unique_ptr<EcCipher>&& obj)
    : object_(std::move(obj))
{}

uint32_t STDCALL EcCipherImp::release()
{
    delete(this); return 0;
}

void* STDCALL EcCipherImp::getObject()
{
    return &object_;
}

uint32_t STDCALL EcCipherImp::init(
    const char* curve)
{
    try
    {
        object_ = std::make_unique<EcCipher>(
            curve);
        return 0;
    }
    catch (const std::exception& e)
    {
        exceptionMessage_ = e.what();
        return -1;
    }
}

uint32_t STDCALL EcCipherImp::getPrimeBitLength(
    uint32_t* _result)
{
    try
    {
        *_result = 
        object_->getPrimeBitLength();
        return 0;
    }
    catch (const std::exception& e)
    {
        exceptionMessage_ = e.what();
        return -1;
    }
}

uint32_t STDCALL EcCipherImp::getPrimeByteLength(
    uint32_t* _result)
{
    try
    {
        *_result = 
        object_->getPrimeByteLength();
        return 0;
    }
    catch (const std::exception& e)
    {
        exceptionMessage_ = e.what();
        return -1;
    }
}

uint32_t STDCALL EcCipherImp::getPrime(
    uint8_t* p,
    uint32_t p_size)
{
    try
    {
        object_->getPrime(
            p,
            p_size);
        return 0;
    }
    catch (const std::exception& e)
    {
        exceptionMessage_ = e.what();
        return -1;
    }
}

uint32_t STDCALL EcCipherImp::getCurvePointCoordinateBitLength(
    uint32_t* _result)
{
    try
    {
        *_result = 
        object_->getCurvePointCoordinateBitLength();
        return 0;
    }
    catch (const std::exception& e)
    {
        exceptionMessage_ = e.what();
        return -1;
    }
}

uint32_t STDCALL EcCipherImp::getCurvePointCoordinateByteLength(
    uint32_t* _result)
{
    try
    {
        *_result = 
        object_->getCurvePointCoordinateByteLength();
        return 0;
    }
    catch (const std::exception& e)
    {
        exceptionMessage_ = e.what();
        return -1;
    }
}

uint32_t STDCALL EcCipherImp::generatePublicKey(
    uint8_t* qx,
    uint32_t qx_size,
    uint8_t* qy,
    uint32_t qy_size,
    const uint8_t* sk,
    uint32_t sk_size)
{
    try
    {
        object_->generatePublicKey(
            qx,
            qx_size,
            qy,
            qy_size,
            sk,
            sk_size);
        return 0;
    }
    catch (const std::exception& e)
    {
        exceptionMessage_ = e.what();
        return -1;
    }
}

uint32_t STDCALL EcCipherImp::generateSignature(
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
    try
    {
        object_->generateSignature(
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
        return 0;
    }
    catch (const std::exception& e)
    {
        exceptionMessage_ = e.what();
        return -1;
    }
}

uint32_t STDCALL EcCipherImp::verifySignature(
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
    uint32_t qy_size)
{
    try
    {
        *_result = 
        object_->verifySignature(
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
        return 0;
    }
    catch (const std::exception& e)
    {
        exceptionMessage_ = e.what();
        return -1;
    }
}

}
extern "C" LCFR_API uint32_t lcfr_EcCipher_release(lcfr_EcCipher_vtable_ptr* this_ptr)
{
    return ((lcfr::IEcCipher*)this_ptr)->release();
}
extern "C" LCFR_API uint32_t lcfr_EcCipher_getExceptionMessage(char const ** _result)
{
    *_result = lcfr::EcCipherImp::exceptionMessage_.c_str();
    return 0;
}
extern "C" LCFR_API uint32_t lcfr_EcCipher_create(
    lcfr_EcCipher_vtable_ptr** _result,
    const char* curve)
{
    try
    {
        *_result = (lcfr_EcCipher_vtable_ptr*) new lcfr::EcCipherImp();
    }
    catch (const std::exception& e)
    {
        lcfr::EcCipherImp::exceptionMessage_ = e.what();
        return -1;
    }
    return ((lcfr::EcCipherImp*)*_result)->init(
        curve);
}
extern "C" LCFR_API uint32_t lcfr_EcCipher_getPrimeBitLength(
    lcfr_EcCipher_vtable_ptr* this_ptr,
    uint32_t* _result)
{
    return ((lcfr::EcCipherImp*)this_ptr)->getPrimeBitLength(
        _result);
}
extern "C" LCFR_API uint32_t lcfr_EcCipher_getPrimeByteLength(
    lcfr_EcCipher_vtable_ptr* this_ptr,
    uint32_t* _result)
{
    return ((lcfr::EcCipherImp*)this_ptr)->getPrimeByteLength(
        _result);
}
extern "C" LCFR_API uint32_t lcfr_EcCipher_getPrime(
    lcfr_EcCipher_vtable_ptr* this_ptr,
    uint8_t* p,
    uint32_t p_size)
{
    return ((lcfr::EcCipherImp*)this_ptr)->getPrime(
        p,
        p_size);
}
extern "C" LCFR_API uint32_t lcfr_EcCipher_getCurvePointCoordinateBitLength(
    lcfr_EcCipher_vtable_ptr* this_ptr,
    uint32_t* _result)
{
    return ((lcfr::EcCipherImp*)this_ptr)->getCurvePointCoordinateBitLength(
        _result);
}
extern "C" LCFR_API uint32_t lcfr_EcCipher_getCurvePointCoordinateByteLength(
    lcfr_EcCipher_vtable_ptr* this_ptr,
    uint32_t* _result)
{
    return ((lcfr::EcCipherImp*)this_ptr)->getCurvePointCoordinateByteLength(
        _result);
}
extern "C" LCFR_API uint32_t lcfr_EcCipher_generatePublicKey(
    lcfr_EcCipher_vtable_ptr* this_ptr,
    uint8_t* qx,
    uint32_t qx_size,
    uint8_t* qy,
    uint32_t qy_size,
    const uint8_t* sk,
    uint32_t sk_size)
{
    return ((lcfr::EcCipherImp*)this_ptr)->generatePublicKey(
        qx,
        qx_size,
        qy,
        qy_size,
        sk,
        sk_size);
}
extern "C" LCFR_API uint32_t lcfr_EcCipher_generateSignature(
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
    uint32_t sk_size)
{
    return ((lcfr::EcCipherImp*)this_ptr)->generateSignature(
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
}
extern "C" LCFR_API uint32_t lcfr_EcCipher_verifySignature(
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
    uint32_t qy_size)
{
    return ((lcfr::EcCipherImp*)this_ptr)->verifySignature(
        _result,
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
}
