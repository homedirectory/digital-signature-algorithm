#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sign.h"
#include "numbers.h"

int main(int argc, char** argv) {
    if (argc < 4) {
        printf("usage: %s sig_file secret_key_file message\n", argv[0]);
        exit(1);
    }

    const char *sig_fname = argv[1], *sk_fname = argv[2], *msg = argv[3];

#define ASSERT_NOT_EMPTY(id, name) {\
    if (strlen(id) == 0) {\
        fprintf(stderr, "["name"]"" should not be empty");\
        exit(1);\
    }\
}
    ASSERT_NOT_EMPTY(sig_fname, "sig_file");
    ASSERT_NOT_EMPTY(sk_fname, "secret_key_file");
    ASSERT_NOT_EMPTY(msg, "message");

    uint64 q, p, g, sk;
    if (read_key(sk_fname, &q, &p, &g, &sk) == -1) {
        fprintf(stderr, "failed to read key from %s\n", sk_fname);
        exit(1);
    }
    PRINT64(sk);

    Signature sig;
    Signature_init(&sig, q, p, g, sk, msg);

    PRINT64(sig.r);
    PRINT64(sig.s);

    if (Signature_to_file(&sig, sig_fname)) {
        fprintf(stderr, "Failed to write signature to file \"%s\"", sig_fname);
        exit(1);
    };
}
