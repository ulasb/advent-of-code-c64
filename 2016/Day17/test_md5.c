#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "md5.h"

int errors = 0;

void print_digest(unsigned char *digest) {
    int i;
    for(i = 0; i < 16; i++) {
        cprintf("%02x", digest[i]);
    }
}

void verify_md5_bytes(const unsigned char *bytes, unsigned int len, const char *label, char *expected) {
    MD5_CTX context;
    unsigned char digest[16];
    char output[33];
    int i;

    MD5Init(&context);
    MD5Update(&context, bytes, len);
    MD5Final(digest, &context);

    // Format digest into output string
    for(i = 0; i < 16; i++) {
        sprintf(output + (i * 2), "%02x", digest[i]);
    }
    output[32] = 0;

    cprintf("MD5(%s) = ", label);
    print_digest(digest);

    if (strcmp(output, expected) == 0) {
        cprintf(" [PASS]\r\n");
    } else {
        cprintf(" [FAIL]\r\n");
        cprintf("Expected: %s\r\n", expected);
        errors++;
    }
}

int main() {
    unsigned char a_byte[] = { 0x61 }; // 'a' in ASCII
    unsigned char abc_bytes[] = { 0x61, 0x62, 0x63 }; // "abc" in ASCII

    textcolor(COLOR_WHITE);
    bgcolor(COLOR_BLACK);
    clrscr();

    cprintf("MD5 Test Suite\r\n");
    cprintf("--------------\r\n");

#ifdef MD5_DEBUG
    MD5_Internal_Tests();
#endif

    verify_md5_bytes((unsigned char*)"", 0, "\"\"", "d41d8cd98f00b204e9800998ecf8427e");
    verify_md5_bytes(a_byte, 1, "\"a\"", "0cc175b9c0f1b6a831c399e269772661");
    {
        const char *abc = "abc";
        cprintf("DEBUG: 'abc' literal: %02x %02x %02x\r\n", (uint8_t)abc[0], (uint8_t)abc[1], (uint8_t)abc[2]);
    }
    verify_md5_bytes((unsigned char*)"abc", 3, "abc_literal", "900150983cd24fb0d6963f7d28e17f72");
    verify_md5_bytes((unsigned char*)"ihgpwlah", 8, "ihgpwlah_literal", "9f6a5ee8779b76e1008064a34b7f8e12");
    
    cprintf("\r\n--- Testing 'abc' ---\r\n");
    verify_md5_bytes(abc_bytes, 3, "\"abc\"", "900150983cd24fb0d6963f7d28e17f72");
    cprintf("--- End Testing ---\r\n\r\n");

    if (errors == 0) {
        cprintf("\r\nAll tests passed!\r\n");
    } else {
        cprintf("\r\n%d tests failed.\r\n", errors);
    }

    cprintf("\r\nPress any key to exit.\r\n");
    cgetc();

    return 0;
}
