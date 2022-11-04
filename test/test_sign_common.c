#include "../sign_common.h"
#include <stdio.h>

void test_get_sha1_u64() {
    char sha1[20] = {
        0x50, 0x13, 0x44, 0x12, 0xfb,
        0x32, 0x43, 0x01, 0x2a, 0xab,
        0x39, 0x23, 0xff, 0xee, 0xcb,
        0x88, 0xba, 0xca, 0x0e, 0x2a
    };
    uint64 x = get_sha1_u64(sha1);
    printf("%lu\n", x);
}

int main(int argc, char** argv) {
    test_get_sha1_u64();
}