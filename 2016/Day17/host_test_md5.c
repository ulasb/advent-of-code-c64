#include <stdio.h>
#include <string.h>
#include "md5.h"

void print_hex(const uint8_t *d) {
    for (int i = 0; i < 16; ++i) printf("%02x", d[i]);
}

void test(const char *s, const char *expected) {
    MD5_CTX ctx;
    uint8_t digest[16];

    MD5Init(&ctx);
    MD5Update(&ctx, (const uint8_t*)s, (uint32_t)strlen(s));
    MD5Final(digest, &ctx);

    print_hex(digest);
    printf("  -> %s", s);
    if (expected) printf("  expected: %s", expected);
    printf("\n");
}

int main(void) {
    test("", "d41d8cd98f00b204e9800998ecf8427e");
    test("a", "0cc175b9c0f1b6a831c399e269772661");
    test("abc", "900150983cd24fb0d6963f7d28e17f72");
    test("ihgpwlah", "9f6a5ee8779b76e1008064a34b7f8e12");
    return 0;
}
