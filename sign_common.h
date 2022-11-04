#include "keygen.h"

typedef struct _Signature {
    uint64 r;
    uint64 s;
} Signature;

int read_key(const char* fname, uint64* p, uint64* q, uint64* g, uint64* key);
uint64 sha1_to_u64(uchar* msg);
uint64 sha1_u64_from_data(const char* data);
void Signature_to_str(Signature* sig, char* dst);
