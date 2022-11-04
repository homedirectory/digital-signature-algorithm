#ifndef KEYGEN_H
#define KEYGEN_H 1

#include "numbers.h"

#define RAND_MAX 18446744073709551615 // unsigned 64-bit
#define BITLEN 64 // 64-bit length for all primes
#define BYTELEN 8

#define PRINT64(x) printf(#x" = %lu\n", (x))

void gen_params(uint64* p, uint64* q, uint64* g);
void gen_keys(uint64 p, uint64 q, uint64 g, uint64* pk, uint64* sk);
int key_to_file(uint64 p, uint64 q, uint64 g, uint64 key, const char* fname);

#endif
