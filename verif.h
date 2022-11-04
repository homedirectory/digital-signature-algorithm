#include "sign_common.h"

int isvalid(Signature* sig, uint64 q, uint64 p, uint64 g, uint64 pk, const char* msg);
int Signature_from_file(Signature* sig, const char* fname);
