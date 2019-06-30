/** \file i_library_info.h
  * The file includes the luCIFeRO library info provider C/C++ interface.
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
  * \struct lcfr_LibraryInfo_vtable_ptr_
  *
  * This struct is the C representation of an interface to a library info provider.
  */
typedef struct lcfr_LibraryInfo_vtable_ptr_
{
    void* vtable_;
} lcfr_LibraryInfo_vtable_ptr;

#ifdef __cplusplus
extern "C" {
#endif

/** \brief Output the message of the last error occurred using the lcfr api, in the calling thread.
  * \param[out] _result the address of the pointer to the output string
  * \return 0 if successful, a positive number otherwise
  */
LCFR_API uint32_t lcfr_LibraryInfo_getExceptionMessage(char const ** _result);

/** \brief Destroy the library info object whose interface is passed to the function.
  * \param this_ptr the address of the library info interface
  * \return 0 if successful, a positive number otherwise
  */
LCFR_API uint32_t lcfr_LibraryInfo_release(lcfr_LibraryInfo_vtable_ptr* this_ptr);

/** \brief Create a LibraryInfo.
*   \param[out] _result the address of the pointer to the library info interface
  */
LCFR_API uint32_t lcfr_LibraryInfo_create(
    lcfr_LibraryInfo_vtable_ptr** _result);

/** \brief Output the library version string.
  * \param[out] _result the address of the pointer to the output string
  * \return 0 if successful, a positive number otherwise
  */
LCFR_API uint32_t lcfr_LibraryInfo_getVersion(
    lcfr_LibraryInfo_vtable_ptr* this_ptr,
    char const ** _result);

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

struct ILibraryInfo
{
    /** \brief Destroy the library info object.
      * \return 0 if successful, a positive number otherwise
      */
    virtual uint32_t STDCALL release() = 0;
    
    /** \brief Initialize the library info object.
      */
    virtual uint32_t STDCALL init() = 0;
    
    /** \brief Output the library version string.
      * \param[out] _result the address of the pointer to the output string
      * \return 0 if successful, a positive number otherwise
      */
    virtual uint32_t STDCALL getVersion(
        char const ** _result) = 0;
    
};

/**
  * \class LibraryInfoProxy
  *
  * This class implements a library info provider.
  */
class LibraryInfoProxy
{
    lcfr::ILibraryInfo* obj_;
    
    public:
    
    /** \brief Create a LibraryInfoProxy.
      */
    LibraryInfoProxy()
    {
        int code = lcfr_LibraryInfo_create(
            (lcfr_LibraryInfo_vtable_ptr**)&obj_);
        if (code != 0)
        {
            const char* message;
            lcfr_LibraryInfo_getExceptionMessage(&message);
            throw new std::runtime_error(message);
        }
    }
    
    /** \brief Return the library version string.
      */
    std::string getVersion()
    {
        const char* _result;
        int code = obj_->getVersion(
            &_result);
        if (code != 0)
        {
            const char* message;
            lcfr_LibraryInfo_getExceptionMessage(&message);
            throw new std::runtime_error(message);
        }
        return std::string(_result);
    }
    
    ~LibraryInfoProxy()
    {
        obj_->release();
    }
};

#ifndef __BUILD_LCFR_LIBRARY__
typedef LibraryInfoProxy LibraryInfo;
#endif
}
#endif

