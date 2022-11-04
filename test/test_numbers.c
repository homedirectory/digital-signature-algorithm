#include "../numbers.h"
#include <stdio.h>
#include <string.h>

void test_modinv() {
    uint64 a, m;
    a = 7;
    m = 97;
    printf("a = %lu, m = %lu\n", a, m);
    printf("modinv = %lu\n", modinv(a, m));

    a = 14;
    printf("a = %lu, m = %lu\n", a, m);
    printf("modinv = %lu\n", modinv(a, m));
}

void test_uint64_to_hexstr() {
    uint64 x = 0xa3bbcc01452fa9bc;
    const char* expected = "a3bbcc01452fa9bc";
    char hex[17];
    uint64_to_hex(x, hex);
    hex[16] = '\0';
    if (strcmp(expected, hex)) {
        printf("Expected: %s\n", expected);
        printf("Actual  : %s\n", hex);
    } else {
        puts("Success.");
    }
}

int main(int argc, char** argv) {
    //test_modinv();
    test_uint64_to_hexstr();
}
