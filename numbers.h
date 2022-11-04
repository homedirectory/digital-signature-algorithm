#ifndef NUMBERS_H
#define NUMBERS_H 1

#include <stdint.h>

typedef uint64_t uint64;
typedef uint8_t  uchar;

static uint64 max_u64 = ~0;

uint64 gen_prime64(uint64 max);
int isprime(uint64 x);
uint64 rand_u64();
uint64 modpow(uint64 base, uint64 exp, uint64 mod);
uint64 modinv(uint64 a, uint64 mod);
uint64 modmult(uint64 a, uint64 b, uint64 mod);
void uint64_to_hex(uint64 x, char* dst);
char int4_to_hex(uchar x);

#endif
