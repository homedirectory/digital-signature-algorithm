#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "keygen.h"
#include "hash/hash.h"

int debug = 0;

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("usage: %s key_name\n", argv[0]);
        exit(1);
    }
    const char* name = argv[1];

    if (argc > 2 && !strcmp(argv[2], "-d"))
        debug = 1;


    uint64 p, q, g;
    gen_params(&p, &q, &g);
    printf("p = %lu, q = %lu, g = %lu\n", p, q, g);

    uint64 pk, sk;
    gen_keys(p, q, g, &pk, &sk);
    printf("pk = %lu, sk = %lu\n", pk, sk);

    int len = strlen(name);
    char pk_fname[len + 5];
    strcpy(pk_fname, name);
    strcat(pk_fname+len, ".pub");
    pk_fname[len+4] = '\0';
    key_to_file(p, q, g, pk, pk_fname);

    char sk_fname[strlen(name) + 5];
    strcpy(sk_fname, name);
    strcat(sk_fname, ".sec");
    sk_fname[len+4] = '\0';
    key_to_file(p, q, g, sk, sk_fname);
}

void gen_keys(uint64 p, uint64 q, uint64 g, uint64* pk, uint64* sk) {
    // private key
    *sk = (rand_u64() % (q-2)) + 1;
    // public key
    *pk = debug ? modpow(g, 2, p) : modpow(g, *sk, p);
}

void gen_params(uint64* p, uint64* q, uint64* g) {
    while (1) {
        puts("1...");
        *q = gen_prime64(-1);

        // choose prime p, such that p-1 is a multiple of q
        uint64 n = 2;
        uint64 pm1 = *q * n; // p - 1
        while (pm1 < max_u64 && !isprime(pm1+1))
            pm1 = *q * ++n;

        if (pm1 >= max_u64) continue;

        *p = pm1+1;
        break;
    }
    PRINT64(*q);
    PRINT64(*p);

    // in the rare case that *g == 1, try again with a different h
    do {
        puts("2...");
        // choose h randomly from [2, p-2]
        //uint64 h = (rand_u64() % (*p-3)) + 2;
        uint64 h = 3;
        PRINT64(h);

        // compute g
        *g = modpow(h, (*p-1) / *q, *p);
    } while (*g == 1);

    PRINT64(*g);
}

// writes 64-bit key to file
// !!! little-endian architecture is assumed !!!
int key_to_file(uint64 p, uint64 q, uint64 g, uint64 key, const char* fname) {
    FILE* fp = fopen(fname, "w");
    if (fp == NULL) return -1;

    // first write parameters
    fwrite(&p, sizeof(p), 1, fp);
    fwrite(&q, sizeof(q), 1, fp);
    fwrite(&g, sizeof(g), 1, fp);

    // now write key
    fwrite(&key, sizeof(key), 1, fp);

    fclose(fp);
    return 0;
}
