#include "library_info.h"

namespace lcfr {

const char * LCFR_VERSION = "1.0.0";

LibraryInfo::LibraryInfo()
{}

const char* LibraryInfo::getVersion() const
{
    return LCFR_VERSION;
}

}
