#include <stdio.h>
#include <string.h>
#include "sign_common.h"
#include "hash/hash.h"

// reads key from file
// little-endian is assumed
int read_key(const char* fname, uint64* p, uint64* q, uint64* g, uint64* key) {
    FILE* fp = fopen(fname, "r");
    if (fp == NULL) return -1;

    if (fread(p, BYTELEN, 1, fp) != 1)
        return -1;
    if (fread(q, BYTELEN, 1, fp) != 1)
        return -1;
    if (fread(g, BYTELEN, 1, fp) != 1)
        return -1;
    if (fread(key, BYTELEN, 1, fp) != 1)
        return -1;

    fclose(fp);
    return 0;
}

// returns 64 leftmost bits of sha1 hash as a big-endian number
// hash is a 20 byte array
uint64 sha1_to_u64(uchar* hash) {
    uint64 x = 0;
    for (int i = 0; i < 8; i++) {
        x |= ((uint64) hash[i]) << ((7-i)*8);
    }
    return x;
}

uint64 sha1_u64_from_data(const char* data) {
    // SHA1 hash is a big endian number
    uchar hash[20];
    sha1(data, strlen(data), hash);
    return sha1_to_u64(hash);
}

void Signature_to_str(Signature* sig, char* dst) {
    char buf[16];
    uint64_to_hex(sig->r, buf);
    memcpy(dst, buf, 16);

    uint64_to_hex(sig->s, buf);
    memcpy(dst+16, buf, 16);

    dst[32] = '\0';
}
