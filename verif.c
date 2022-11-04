#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "numbers.h"
#include "hash/hash.h"
#include "verif.h"

int main(int argc, char** argv) {
    if (argc < 4) {
        printf("usage: %s sig_file pub_key_file message\n", argv[0]);
        exit(1);
    }

    const char *sig_fname = argv[1], *pk_fname = argv[2], *msg = argv[3];

#define ASSERT_NOT_EMPTY(id, name) {\
    if (strlen(id) == 0) {\
        fprintf(stderr, "["name"]"" should not be empty");\
        exit(1);\
    }\
}
    ASSERT_NOT_EMPTY(sig_fname, "sig_file");
    ASSERT_NOT_EMPTY(pk_fname, "secret_key_file");
    ASSERT_NOT_EMPTY(msg, "message");

    uint64 q, p, g, pk;
    if (read_key(pk_fname, &q, &p, &g, &pk) == -1) {
        fprintf(stderr, "failed to read key from %s\n", pk_fname);
        exit(1);
    }
    PRINT64(pk);

    Signature sig;
    if (Signature_from_file(&sig, sig_fname)) {
        fprintf(stderr, "Failed to read signature from file \"%s\"", sig_fname);
        exit(1);
    }
    PRINT64(sig.r);
    PRINT64(sig.s);

    if (!isvalid(&sig, q, p, g, pk, msg)) {
        puts("Invalid signature!");
    } else {
        puts("Signature is valid.");
    }

}

int isvalid(Signature* sig, uint64 q, uint64 p, uint64 g, uint64 pk, const char* msg) {
    uint64 r = sig->r;
    uint64 s = sig->s;

    // 1
    if (r >= q || s >=q) return 0;
    // 2
    uint64 w = modinv(s, q);
    // 3
    uint64 h = sha1_u64_from_data(msg);
    uint64 u1 = modmult(h, w, q);
    // 4
    uint64 u2 = modmult(r, w, q);
    // 5
    uint64 v = ((modpow(g, u1, p) * (modpow(pk, u2, p))) %p) % q;

    return v == r;
}

// little-endian is assumed
int Signature_from_file(Signature* sig, const char* fname) {
    FILE* fp = fopen(fname, "r");
    if (fp == NULL) return -1;

    if (fread(&(sig->r), sizeof(sig->r), 1, fp) != 1)
        return -1;
    if (fread(&(sig->s), sizeof(sig->s), 1, fp) != 1)
        return -1;

    fclose(fp);
    return 0;
}
