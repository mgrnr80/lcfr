#pragma once

#include <stdint.h>
#include <memory>
#include <string>
#include <exception>
#include <atomic>
#include "lcfr/i_library_info.h"
#include "lcfr/library_info.h"

namespace lcfr {

struct LibraryInfoImp : public ILibraryInfo
{
    static thread_local std::string exceptionMessage_;
    std::unique_ptr<LibraryInfo> object_;
    
    LibraryInfoImp();
    LibraryInfoImp(std::unique_ptr<LibraryInfo>&& obj);
    
    virtual uint32_t STDCALL release();
    
    void* STDCALL getObject();
    
    virtual uint32_t STDCALL init();
    
    virtual uint32_t STDCALL getVersion(
        char const ** _result);
    
};

}

