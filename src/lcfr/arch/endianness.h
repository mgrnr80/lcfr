#pragma once

#include <stdint.h>
#include <stdlib.h>

namespace lcfr {

uint16_t reverse(uint16_t x);
uint32_t reverse(uint32_t x);
uint64_t reverse(uint64_t x);

#if BIG_ENDIAN
static void be_serialize(uint8_t* to, uint64_t from)
{
    *(uint64_t*)to = from;
}

static void be_serialize(uint8_t* to, uint32_t from)
{
    *(uint32_t*)to = from;
}

static void be_serialize(uint8_t* to, uint16_t from)
{
    *(uint16_t*)to = from;
}

static void be_deserialize(uint64_t& to, const uint8_t* from)
{
    to = *(uint64_t*)from;
}

static void be_deserialize(uint32_t& to, const uint8_t* from)
{
    to = *(uint32_t*)from;
}

static void be_deserialize(uint16_t& to, const uint8_t* from)
{
    to = *(uint16_t*)from;
}
#else
static void be_serialize(uint8_t* to, uint64_t from)
{
    *(uint64_t*)to = reverse(from);
}

static void be_serialize(uint8_t* to, uint32_t from)
{
    *(uint32_t*)to = reverse(from);
}

static void be_serialize(uint8_t* to, uint16_t from)
{
    *(uint16_t*)to = reverse(from);
}

static void be_deserialize(uint64_t& to, const uint8_t* from)
{
    to = reverse(*(uint64_t*)from);
}

static void be_deserialize(uint32_t& to, const uint8_t* from)
{
    to = reverse(*(uint32_t*)from);
}

static void be_deserialize(uint16_t& to, const uint8_t* from)
{
    to = reverse(*(uint16_t*)from);
}
#endif

}
