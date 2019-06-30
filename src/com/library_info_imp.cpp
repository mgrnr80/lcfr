#include <memory.h>
#include "com/library_info_imp.h"

namespace lcfr {

thread_local std::string LibraryInfoImp::exceptionMessage_;

LibraryInfoImp::LibraryInfoImp()
{}

LibraryInfoImp::LibraryInfoImp(std::unique_ptr<LibraryInfo>&& obj)
    : object_(std::move(obj))
{}

uint32_t STDCALL LibraryInfoImp::release()
{
    delete(this); return 0;
}

void* STDCALL LibraryInfoImp::getObject()
{
    return &object_;
}

uint32_t STDCALL LibraryInfoImp::init()
{
    try
    {
        object_ = std::make_unique<LibraryInfo>();
        return 0;
    }
    catch (const std::exception& e)
    {
        exceptionMessage_ = e.what();
        return -1;
    }
}

uint32_t STDCALL LibraryInfoImp::getVersion(
    char const ** _result)
{
    try
    {
        *_result = 
        object_->getVersion();
        return 0;
    }
    catch (const std::exception& e)
    {
        exceptionMessage_ = e.what();
        return -1;
    }
}

}
extern "C" LCFR_API uint32_t lcfr_LibraryInfo_release(lcfr_LibraryInfo_vtable_ptr* this_ptr)
{
    return ((lcfr::ILibraryInfo*)this_ptr)->release();
}
extern "C" LCFR_API uint32_t lcfr_LibraryInfo_getExceptionMessage(char const ** _result)
{
    *_result = lcfr::LibraryInfoImp::exceptionMessage_.c_str();
    return 0;
}
extern "C" LCFR_API uint32_t lcfr_LibraryInfo_create(
    lcfr_LibraryInfo_vtable_ptr** _result)
{
    try
    {
        *_result = (lcfr_LibraryInfo_vtable_ptr*) new lcfr::LibraryInfoImp();
    }
    catch (const std::exception& e)
    {
        lcfr::LibraryInfoImp::exceptionMessage_ = e.what();
        return -1;
    }
    return ((lcfr::LibraryInfoImp*)*_result)->init();
}
extern "C" LCFR_API uint32_t lcfr_LibraryInfo_getVersion(
    lcfr_LibraryInfo_vtable_ptr* this_ptr,
    char const ** _result)
{
    return ((lcfr::LibraryInfoImp*)this_ptr)->getVersion(
        _result);
}
