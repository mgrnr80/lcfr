#include <jni.h>
#include <memory>
#include <exception>
#include "lcfr/cipher.h"

extern "C" {

JNIEXPORT void JNICALL Java_lcfr_EcCipher_init__Ljava_lang_String_2(
    JNIEnv *env,
    jobject obj,
    jstring curve)
{
    try
    {
        auto _curve_deleter = [&env, &curve](const char* _ptr){ env->ReleaseStringUTFChars(curve, _ptr); };
        std::unique_ptr<const char[], decltype(_curve_deleter)> _curve(env->GetStringUTFChars(curve, nullptr), _curve_deleter);
        lcfr::EcCipher* cpp_this = new lcfr::EcCipher(
            _curve.get());
        env->SetLongField(obj, env->GetFieldID(env->GetObjectClass(obj), "cpp_this", "J"), (jlong)cpp_this);
    }
    catch(const std::exception& e)
    {
        env->ThrowNew(env->FindClass("java/lang/Exception"), e.what());
    }
}

JNIEXPORT jint JNICALL Java_lcfr_EcCipher_getPrimeBitLength__(
    JNIEnv *env,
    jobject obj)
{
    try
    {
        lcfr::EcCipher* cpp_this = (lcfr::EcCipher*)
            env->GetLongField(obj, env->GetFieldID(env->GetObjectClass(obj), "cpp_this", "J"));
        auto _result = cpp_this->getPrimeBitLength();
        return _result;
    }
    catch(const std::exception& e)
    {
        env->ThrowNew(env->FindClass("java/lang/Exception"), e.what());
    }
    return jint(); // to suppress warning
}

JNIEXPORT jint JNICALL Java_lcfr_EcCipher_getPrimeByteLength__(
    JNIEnv *env,
    jobject obj)
{
    try
    {
        lcfr::EcCipher* cpp_this = (lcfr::EcCipher*)
            env->GetLongField(obj, env->GetFieldID(env->GetObjectClass(obj), "cpp_this", "J"));
        auto _result = cpp_this->getPrimeByteLength();
        return _result;
    }
    catch(const std::exception& e)
    {
        env->ThrowNew(env->FindClass("java/lang/Exception"), e.what());
    }
    return jint(); // to suppress warning
}

JNIEXPORT void JNICALL Java_lcfr_EcCipher_getPrime___3B(
    JNIEnv *env,
    jobject obj,
    jbyteArray p)
{
    try
    {
        auto _p_deleter = [&env, &p](uint8_t* _ptr){ env->ReleaseByteArrayElements(p, (jbyte*)_ptr, 0); };
        std::unique_ptr<uint8_t[], decltype(_p_deleter)> _p((uint8_t*)env->GetByteArrayElements(p, nullptr), _p_deleter);
        lcfr::EcCipher* cpp_this = (lcfr::EcCipher*)
            env->GetLongField(obj, env->GetFieldID(env->GetObjectClass(obj), "cpp_this", "J"));
        cpp_this->getPrime(
            _p.get(),
            (uint32_t)env->GetArrayLength(p));
    }
    catch(const std::exception& e)
    {
        env->ThrowNew(env->FindClass("java/lang/Exception"), e.what());
    }
}

JNIEXPORT jint JNICALL Java_lcfr_EcCipher_getCurvePointCoordinateBitLength__(
    JNIEnv *env,
    jobject obj)
{
    try
    {
        lcfr::EcCipher* cpp_this = (lcfr::EcCipher*)
            env->GetLongField(obj, env->GetFieldID(env->GetObjectClass(obj), "cpp_this", "J"));
        auto _result = cpp_this->getCurvePointCoordinateBitLength();
        return _result;
    }
    catch(const std::exception& e)
    {
        env->ThrowNew(env->FindClass("java/lang/Exception"), e.what());
    }
    return jint(); // to suppress warning
}

JNIEXPORT jint JNICALL Java_lcfr_EcCipher_getCurvePointCoordinateByteLength__(
    JNIEnv *env,
    jobject obj)
{
    try
    {
        lcfr::EcCipher* cpp_this = (lcfr::EcCipher*)
            env->GetLongField(obj, env->GetFieldID(env->GetObjectClass(obj), "cpp_this", "J"));
        auto _result = cpp_this->getCurvePointCoordinateByteLength();
        return _result;
    }
    catch(const std::exception& e)
    {
        env->ThrowNew(env->FindClass("java/lang/Exception"), e.what());
    }
    return jint(); // to suppress warning
}

JNIEXPORT void JNICALL Java_lcfr_EcCipher_generatePublicKey___3B_3B_3B(
    JNIEnv *env,
    jobject obj,
    jbyteArray qx,
    jbyteArray qy,
    jbyteArray sk)
{
    try
    {
        auto _qx_deleter = [&env, &qx](uint8_t* _ptr){ env->ReleaseByteArrayElements(qx, (jbyte*)_ptr, 0); };
        std::unique_ptr<uint8_t[], decltype(_qx_deleter)> _qx((uint8_t*)env->GetByteArrayElements(qx, nullptr), _qx_deleter);
        auto _qy_deleter = [&env, &qy](uint8_t* _ptr){ env->ReleaseByteArrayElements(qy, (jbyte*)_ptr, 0); };
        std::unique_ptr<uint8_t[], decltype(_qy_deleter)> _qy((uint8_t*)env->GetByteArrayElements(qy, nullptr), _qy_deleter);
        auto _sk_deleter = [&env, &sk](uint8_t* _ptr){ env->ReleaseByteArrayElements(sk, (jbyte*)_ptr, JNI_ABORT); };
        std::unique_ptr<uint8_t[], decltype(_sk_deleter)> _sk((uint8_t*)env->GetByteArrayElements(sk, nullptr), _sk_deleter);
        lcfr::EcCipher* cpp_this = (lcfr::EcCipher*)
            env->GetLongField(obj, env->GetFieldID(env->GetObjectClass(obj), "cpp_this", "J"));
        cpp_this->generatePublicKey(
            _qx.get(),
            (uint32_t)env->GetArrayLength(qx),
            _qy.get(),
            (uint32_t)env->GetArrayLength(qy),
            _sk.get(),
            (uint32_t)env->GetArrayLength(sk));
    }
    catch(const std::exception& e)
    {
        env->ThrowNew(env->FindClass("java/lang/Exception"), e.what());
    }
}

JNIEXPORT void JNICALL Java_lcfr_EcCipher_generateSignature___3B_3B_3B_3B_3B(
    JNIEnv *env,
    jobject obj,
    jbyteArray r,
    jbyteArray s,
    jbyteArray hash,
    jbyteArray ek,
    jbyteArray sk)
{
    try
    {
        auto _r_deleter = [&env, &r](uint8_t* _ptr){ env->ReleaseByteArrayElements(r, (jbyte*)_ptr, 0); };
        std::unique_ptr<uint8_t[], decltype(_r_deleter)> _r((uint8_t*)env->GetByteArrayElements(r, nullptr), _r_deleter);
        auto _s_deleter = [&env, &s](uint8_t* _ptr){ env->ReleaseByteArrayElements(s, (jbyte*)_ptr, 0); };
        std::unique_ptr<uint8_t[], decltype(_s_deleter)> _s((uint8_t*)env->GetByteArrayElements(s, nullptr), _s_deleter);
        auto _hash_deleter = [&env, &hash](uint8_t* _ptr){ env->ReleaseByteArrayElements(hash, (jbyte*)_ptr, JNI_ABORT); };
        std::unique_ptr<uint8_t[], decltype(_hash_deleter)> _hash((uint8_t*)env->GetByteArrayElements(hash, nullptr), _hash_deleter);
        auto _ek_deleter = [&env, &ek](uint8_t* _ptr){ env->ReleaseByteArrayElements(ek, (jbyte*)_ptr, JNI_ABORT); };
        std::unique_ptr<uint8_t[], decltype(_ek_deleter)> _ek((uint8_t*)env->GetByteArrayElements(ek, nullptr), _ek_deleter);
        auto _sk_deleter = [&env, &sk](uint8_t* _ptr){ env->ReleaseByteArrayElements(sk, (jbyte*)_ptr, JNI_ABORT); };
        std::unique_ptr<uint8_t[], decltype(_sk_deleter)> _sk((uint8_t*)env->GetByteArrayElements(sk, nullptr), _sk_deleter);
        lcfr::EcCipher* cpp_this = (lcfr::EcCipher*)
            env->GetLongField(obj, env->GetFieldID(env->GetObjectClass(obj), "cpp_this", "J"));
        cpp_this->generateSignature(
            _r.get(),
            (uint32_t)env->GetArrayLength(r),
            _s.get(),
            (uint32_t)env->GetArrayLength(s),
            _hash.get(),
            (uint32_t)env->GetArrayLength(hash),
            _ek.get(),
            (uint32_t)env->GetArrayLength(ek),
            _sk.get(),
            (uint32_t)env->GetArrayLength(sk));
    }
    catch(const std::exception& e)
    {
        env->ThrowNew(env->FindClass("java/lang/Exception"), e.what());
    }
}

JNIEXPORT jint JNICALL Java_lcfr_EcCipher_verifySignature___3B_3B_3B_3B_3B(
    JNIEnv *env,
    jobject obj,
    jbyteArray r,
    jbyteArray s,
    jbyteArray hash,
    jbyteArray qx,
    jbyteArray qy)
{
    try
    {
        auto _r_deleter = [&env, &r](uint8_t* _ptr){ env->ReleaseByteArrayElements(r, (jbyte*)_ptr, JNI_ABORT); };
        std::unique_ptr<uint8_t[], decltype(_r_deleter)> _r((uint8_t*)env->GetByteArrayElements(r, nullptr), _r_deleter);
        auto _s_deleter = [&env, &s](uint8_t* _ptr){ env->ReleaseByteArrayElements(s, (jbyte*)_ptr, JNI_ABORT); };
        std::unique_ptr<uint8_t[], decltype(_s_deleter)> _s((uint8_t*)env->GetByteArrayElements(s, nullptr), _s_deleter);
        auto _hash_deleter = [&env, &hash](uint8_t* _ptr){ env->ReleaseByteArrayElements(hash, (jbyte*)_ptr, JNI_ABORT); };
        std::unique_ptr<uint8_t[], decltype(_hash_deleter)> _hash((uint8_t*)env->GetByteArrayElements(hash, nullptr), _hash_deleter);
        auto _qx_deleter = [&env, &qx](uint8_t* _ptr){ env->ReleaseByteArrayElements(qx, (jbyte*)_ptr, JNI_ABORT); };
        std::unique_ptr<uint8_t[], decltype(_qx_deleter)> _qx((uint8_t*)env->GetByteArrayElements(qx, nullptr), _qx_deleter);
        auto _qy_deleter = [&env, &qy](uint8_t* _ptr){ env->ReleaseByteArrayElements(qy, (jbyte*)_ptr, JNI_ABORT); };
        std::unique_ptr<uint8_t[], decltype(_qy_deleter)> _qy((uint8_t*)env->GetByteArrayElements(qy, nullptr), _qy_deleter);
        lcfr::EcCipher* cpp_this = (lcfr::EcCipher*)
            env->GetLongField(obj, env->GetFieldID(env->GetObjectClass(obj), "cpp_this", "J"));
        auto _result = cpp_this->verifySignature(
            _r.get(),
            (uint32_t)env->GetArrayLength(r),
            _s.get(),
            (uint32_t)env->GetArrayLength(s),
            _hash.get(),
            (uint32_t)env->GetArrayLength(hash),
            _qx.get(),
            (uint32_t)env->GetArrayLength(qx),
            _qy.get(),
            (uint32_t)env->GetArrayLength(qy));
        return _result;
    }
    catch(const std::exception& e)
    {
        env->ThrowNew(env->FindClass("java/lang/Exception"), e.what());
    }
    return jint(); // to suppress warning
}

JNIEXPORT void JNICALL Java_lcfr_EcCipher_destroy__(
    JNIEnv *env,
    jobject obj)
{
    try
    {
        auto cpp_this = (lcfr::EcCipher*) 
            env->GetLongField(obj, env->GetFieldID(env->GetObjectClass(obj), "cpp_this", "J"));
        if (cpp_this) {
            delete(cpp_this);
            env->SetLongField(obj, env->GetFieldID(env->GetObjectClass(obj), "cpp_this", "J"), (jlong)nullptr);
        }
    }
    catch(const std::exception& e)
    {
        env->ThrowNew(env->FindClass("java/lang/Exception"), e.what());
    }
}
}
