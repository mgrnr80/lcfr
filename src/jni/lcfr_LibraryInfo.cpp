#include <jni.h>
#include <memory>
#include <exception>
#include "lcfr/library_info.h"

extern "C" {

JNIEXPORT void JNICALL Java_lcfr_LibraryInfo_init__(
    JNIEnv *env,
    jobject obj)
{
    try
    {
        lcfr::LibraryInfo* cpp_this = new lcfr::LibraryInfo();
        env->SetLongField(obj, env->GetFieldID(env->GetObjectClass(obj), "cpp_this", "J"), (jlong)cpp_this);
    }
    catch(const std::exception& e)
    {
        env->ThrowNew(env->FindClass("java/lang/Exception"), e.what());
    }
}

JNIEXPORT jstring JNICALL Java_lcfr_LibraryInfo_getVersion__(
    JNIEnv *env,
    jobject obj)
{
    try
    {
        lcfr::LibraryInfo* cpp_this = (lcfr::LibraryInfo*)
            env->GetLongField(obj, env->GetFieldID(env->GetObjectClass(obj), "cpp_this", "J"));
        auto _result = cpp_this->getVersion();
        return env->NewStringUTF(_result);
    }
    catch(const std::exception& e)
    {
        env->ThrowNew(env->FindClass("java/lang/Exception"), e.what());
    }
    return jstring(); // to suppress warning
}

JNIEXPORT void JNICALL Java_lcfr_LibraryInfo_destroy__(
    JNIEnv *env,
    jobject obj)
{
    try
    {
        auto cpp_this = (lcfr::LibraryInfo*) 
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
