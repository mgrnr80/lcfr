#pragma once

#include <stdint.h>
#include <string.h>

namespace lcfr {
template <class W>
struct uint_traits {
};

template <>
struct uint_traits<uint32_t>
{
    typedef uint64_t d;
    typedef int64_t sd;
    typedef int32_t s;
    enum : unsigned { bits = 8 * sizeof(uint32_t) };
};

template <>
struct uint_traits<uint16_t>
{
    typedef uint32_t d;
    typedef int32_t sd;
    typedef int16_t s;
    enum : unsigned { bits = 8 * sizeof(uint16_t) };
};

template <>
struct uint_traits<uint8_t>
{
    typedef uint16_t d;
    typedef int16_t sd;
    typedef int8_t s;
    enum : unsigned { bits = 8 * sizeof(uint8_t) };
};


void zero(uint32_t* x, size_t n);
void set(uint32_t* x, const uint32_t* a, size_t n);
void set(uint32_t* x, const uint32_t* a, size_t na, size_t nb);

uint32_t add(uint32_t* x, const uint32_t* a, const uint32_t* b, size_t n);
uint32_t add(uint32_t* x, const uint32_t* a, const uint32_t* b, size_t na, size_t nb);
uint32_t add(uint32_t* x, const uint32_t* a, uint32_t b, size_t n);

int32_t sub(uint32_t* x, const uint32_t* a, const uint32_t* b, size_t n);
int32_t sub(uint32_t* x, const uint32_t* a, const uint32_t* b, size_t na, size_t nb);

void mult(uint32_t* x, const uint32_t* a, const uint32_t* b, size_t n);
void mult(uint32_t* x, const uint32_t* a, const uint32_t* b, size_t na, size_t nb);
void square(uint32_t* x, const uint32_t* a, size_t na);

uint32_t mult_add(uint32_t* x, const uint32_t* a, const uint32_t* b, uint32_t m, size_t na, size_t nb);

void shift_right(uint32_t* x, const uint32_t* a, size_t b, size_t n);
void shift_left(uint32_t* x, const uint32_t* a, size_t b, size_t n);
void shift_left(uint32_t* x, size_t b, size_t n);

void bitwise_xor(uint32_t* x, const uint32_t* a, const uint32_t* b, size_t n);
void bitwise_and(uint32_t* x, const uint32_t* a, const uint32_t* b, size_t n);
void bitwise_or(uint32_t* x, const uint32_t* a, const uint32_t* b, size_t n);

uint32_t carry(const uint32_t* a, const uint32_t* b, size_t na, size_t nb);

void barret(uint32_t* x, const uint32_t* p, const uint32_t* m, const uint32_t* r, size_t n, size_t nm, size_t nr, uint32_t* t);
void barret(uint32_t* x, const uint32_t* prod, const uint32_t* prime, const uint32_t* r, size_t nw, size_t npb, uint32_t* t);

uint32_t inverse(uint32_t x);
uint32_t inverse(uint32_t x, uint32_t mod);

bool eq(const uint32_t* a, const uint32_t* b, size_t n);
bool g(const uint32_t* a, const uint32_t* b, size_t n);
bool l(const uint32_t* a, const uint32_t* b, size_t n);
bool ge(const uint32_t* a, const uint32_t* b, size_t n);
bool le(const uint32_t* a, const uint32_t* b, size_t n);

// ------------------------------------------------------------------
void zero(uint16_t* x, size_t n);
void set(uint16_t* x, const uint16_t* a, size_t n);
void set(uint16_t* x, const uint16_t* a, size_t na, size_t nb);

uint16_t add(uint16_t* x, const uint16_t* a, const uint16_t* b, size_t n);
uint16_t add(uint16_t* x, const uint16_t* a, const uint16_t* b, size_t na, size_t nb);
uint16_t add(uint16_t* x, const uint16_t* a, uint16_t b, size_t n);

int16_t sub(uint16_t* x, const uint16_t* a, const uint16_t* b, size_t n);
int16_t sub(uint16_t* x, const uint16_t* a, const uint16_t* b, size_t na, size_t nb);

void mult(uint16_t* x, const uint16_t* a, const uint16_t* b, size_t n);
void mult(uint16_t* x, const uint16_t* a, const uint16_t* b, size_t na, size_t nb);
void square(uint16_t* x, const uint16_t* a, size_t na);

uint16_t mult_add(uint16_t* x, const uint16_t* a, const uint16_t* b, uint16_t m, size_t na, size_t nb);

void shift_right(uint16_t* x, const uint16_t* a, size_t b, size_t n);
void shift_left(uint16_t* x, const uint16_t* a, size_t b, size_t n);
void shift_left(uint16_t* x, size_t b, size_t n);

void bitwise_xor(uint16_t* x, const uint16_t* a, const uint16_t* b, size_t n);
void bitwise_and(uint16_t* x, const uint16_t* a, const uint16_t* b, size_t n);
void bitwise_or(uint16_t* x, const uint16_t* a, const uint16_t* b, size_t n);

uint16_t carry(const uint16_t* a, const uint16_t* b, size_t na, size_t nb);

void barret(uint16_t* x, const uint16_t* p, const uint16_t* m, const uint16_t* r, size_t n, size_t nm, size_t nr, uint16_t* t);
void barret(uint16_t* x, const uint16_t* prod, const uint16_t* prime, const uint16_t* r, size_t nw, size_t npb, uint16_t* t);

uint16_t inverse(uint16_t x);
uint16_t inverse(uint16_t x, uint16_t mod);

bool eq(const uint16_t* a, const uint16_t* b, size_t n);
bool g(const uint16_t* a, const uint16_t* b, size_t n);
bool l(const uint16_t* a, const uint16_t* b, size_t n);
bool ge(const uint16_t* a, const uint16_t* b, size_t n);
bool le(const uint16_t* a, const uint16_t* b, size_t n);

}
