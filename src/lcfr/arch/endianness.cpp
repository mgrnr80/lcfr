#include "lcfr/arch/endianness.h"

uint16_t lcfr::reverse(uint16_t x)
{
    return (x << 8) | (x >> 8);
}

uint32_t lcfr::reverse(uint32_t x)
{
    return reverse(uint16_t(x >> 16)) | uint32_t(reverse(uint16_t(x))) << 16;
}

uint64_t lcfr::reverse(uint64_t x)
{
    return reverse(uint32_t(x >> 32)) | uint64_t(reverse(uint32_t(x))) << 32;
}
