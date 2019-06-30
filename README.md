# luCIFeRO

luCIFeRO is a C++ library that implements elliptic curve cryptography algorithms for digital signature (ECDSA).

The library supports SEC curves up to 128 bit security. The list includes:
 * 56-bit - secp112r1, secp112r2
 * 64-bit - secp128r1, secp128r2
 * 80-bit - secp160k1, secp160r1
 * 96-bit - secp192k1, secp192r1
 * 128-bit - secp256k1, secp256r1
 
Compiling the project yields a shared object or dynamic library that can be linked to C/C++ modules, however the core of the library is small enough to be embedded in a C++ project.

C example code:
```C
#define NP 32
...
uint8_t r[NP], s[NP], hash[NP]; // ECDSA signature components, hash code
uint8_t ek[NP], sk[NP]; // ephemeral key, secret key
...
void* cipher; lcfr_EcCipher_create(&cipher, "secp256k1");
lcfr_EcCipher_generateSignature(cipher, r, NP, s, NP, hash, NP, ek, NP, sk, NP);
lcfr_EcCipher_release(cipher);
```
