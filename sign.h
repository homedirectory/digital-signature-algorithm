#include "sign_common.h"

void Signature_init(Signature* sig, uint64 q, uint64 p, uint64 g, uint64 sk, const char* msg);
int Signature_to_file(Signature* sig, const char* fname);
