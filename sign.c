#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sign.h"
#include "numbers.h"

void Signature_init(Signature* sig, uint64 q, uint64 p, uint64 g, uint64 sk, const char* msg) {
    // 64 leftmost bits of the message hash are needed
    uint64 h = sha1_u64_from_data(msg);
    uint64 r, s;
    while (1) {
        // choose k randomly from [1, q-1]
        uint64 k = (rand_u64() % (q-2)) + 1;

        // compute r, must not be 0
        r = modpow(g, k, p) % q;
        if (r == 0) continue;

        // compute s, must not be 0
        uint64 sum = h + (sk*r);
        s = (modinv(k, q) * (sum % q)) % q;
        if (s == 0) continue;
        break;
    }

    sig->r = r;
    sig->s = s;
}

int Signature_to_file(Signature* sig, const char* fname) {
    FILE* fp = fopen(fname, "w");
    if (fp == NULL) return -1;

    uint64 r = sig->r;
    uint64 s = sig->s;
    if (fwrite(&r, sizeof(r), 1, fp) != 1)
        return -1;
    if (fwrite(&s, sizeof(s), 1, fp) != 1)
        return -1;

    fclose(fp);
    return 0;
}
