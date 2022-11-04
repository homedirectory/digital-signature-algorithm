#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include "numbers.h"
#include "hash/hash.h"

// generate 64-bit prime <= max
// if max == -1, then no limit is set
uint64 gen_prime64(uint64 max) {
    srand(time(NULL));

    max = max == -1 ? max_u64 : max;
    uint64 r = rand();
    while (r < 2 || r > max || !isprime(r))
        r = rand();

    return r;
}

int isprime(uint64 x) {
    if (x < 2)
        return 0;

    uint64 sup = sqrt(x);
    for (uint64 i = 2; i < sup; i++) {
        if (x % i == 0)
            return 0;
    }

    return 1;
}

uint64 rand_u64() {
    srand(time(NULL));
    return (uint64) rand();
}

// modular exponentiation
uint64 modpow(uint64 base, uint64 exp, uint64 mod) {
    if (mod == 1) return 0;

    uint64 c = 1, e = 0;
    while (e++ < exp) {
        c = (base * c) % mod;
    }
    return c;
}

uint64 modinv(uint64 a, uint64 mod) {
    //printf("modinv(%lu, %lu)\n", a, mod);
    // a and mod must be coprime
    // extended eucledian algorithm
    uint64 r0 = a, r1 = mod, r;
    uint64 s0 = 1, s1 = 0,   s;
    uint64 t0 = 0, t1 = 1,   t;
    uint64 q;

    uint64 ri, si, ti;
    do {
        q = r0 / r1;      // quotient

        ri = r1;
        r1 = r0 - (r1*q);  // remainder
        r0 = ri;

        si = s1;
        s1 = s0 - (q*s1);
        s0 = si;

        ti = t1;
        t1 = t0 - (q*t1);
        t0 = ti;
    } while (r1 > 0);
    //printf("-> %lu\n", s0);

    return s0;
}

// calculates (a*b) mod m
uint64 modmult(uint64 a, uint64 b, uint64 m) {
    return ((a % m) * (b % m)) % m;
}

// no NULL byte will be stored at dst
void uint64_to_hex(uint64 x, char* dst) {
    // consider endianness
    int be = isBE();
    int start = be ? 0 : 7;
    int end = be ? 8 : -1;

    uchar* bp = (uchar*) &x;
    for (int i = start, j = 0; i != end;) {
        uchar b = bp[i];
        uchar q1 = (b & 0xf0) >> 4;
        dst[j++] = int4_to_hex(q1);
        uchar q2 = b & 0x0f;
        dst[j++] = int4_to_hex(q2);

        i = be ? i+1 : i-1;
    }
}
