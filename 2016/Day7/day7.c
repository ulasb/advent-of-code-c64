#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ABBA detection: four characters where the first and last are the same,
   the middle two are the same, and the first and middle are different. */
int has_abba(const char *s, int len) {
    int i;
    for (i = 0; i <= len - 4; i++) {
        if (s[i] == s[i + 3] && s[i + 1] == s[i + 2] && s[i] != s[i + 1]) {
            return 1;
        }
    }
    return 0;
}

/* Part 1: IP supports TLS if it has an ABBA outside brackets and no ABBA inside any square brackets. */
int supports_tls(const char *ip) {
    int inside = 0;
    int has_outside_abba = 0;
    int has_inside_abba = 0;
    int i, start;
    int len = strlen(ip);

    start = 0;
    for (i = 0; i <= len; i++) {
        if (ip[i] == '[' || ip[i] == ']' || ip[i] == '\0') {
            int part_len = i - start;
            if (part_len >= 4) {
                if (has_abba(ip + start, part_len)) {
                    if (inside) has_inside_abba = 1;
                    else has_outside_abba = 1;
                }
            }
            if (ip[i] == '[') inside = 1;
            else if (ip[i] == ']') inside = 0;
            start = i + 1;
        }
    }

    return has_outside_abba && !has_inside_abba;
}

/* ABA storage for Part 2 */
unsigned char found_abas[85]; /* 26*26 = 676 bits = 85 bytes */

void clear_abas(void) {
    memset(found_abas, 0, sizeof(found_abas));
}

void set_aba(char a, char b) {
    if (a >= 'a' && a <= 'z' && b >= 'a' && b <= 'z') {
        int idx = (a - 'a') * 26 + (b - 'a');
        found_abas[idx >> 3] |= (1 << (idx & 7));
    }
}

int check_aba(char a, char b) {
    if (a >= 'a' && a <= 'z' && b >= 'a' && b <= 'z') {
        int idx = (a - 'a') * 26 + (b - 'a');
        return (found_abas[idx >> 3] & (1 << (idx & 7))) != 0;
    }
    return 0;
}

/* Part 2: IP supports SSL if it contains an ABA pattern outside brackets
   and a corresponding BAB pattern inside square brackets. */
int supports_ssl(const char *ip) {
    int inside = 0;
    int i, j, start;
    int len = strlen(ip);

    clear_abas();

    /* First pass: find all ABAs outside brackets */
    start = 0;
    inside = 0;
    for (i = 0; i <= len; i++) {
        if (ip[i] == '[' || ip[i] == ']' || ip[i] == '\0') {
            if (!inside) {
                int part_len = i - start;
                for (j = 0; j <= part_len - 3; j++) {
                    const char *s = ip + start + j;
                    if (s[0] == s[2] && s[0] != s[1]) {
                        set_aba(s[0], s[1]);
                    }
                }
            }
            if (ip[i] == '[') inside = 1;
            else if (ip[i] == ']') inside = 0;
            start = i + 1;
        }
    }

    /* Second pass: find all BABs inside brackets and check corresponding ABA */
    start = 0;
    inside = 0;
    for (i = 0; i <= len; i++) {
        if (ip[i] == '[' || ip[i] == ']' || ip[i] == '\0') {
            if (inside) {
                int part_len = i - start;
                for (j = 0; j <= part_len - 3; j++) {
                    const char *s = ip + start + j;
                    if (s[0] == s[2] && s[0] != s[1]) {
                        /* For BAB (s[0], s[1], s[0]), we need ABA (s[1], s[0], s[1]) */
                        if (check_aba(s[1], s[0])) return 1;
                    }
                }
            }
            if (ip[i] == '[') inside = 1;
            else if (ip[i] == ']') inside = 0;
            start = i + 1;
        }
    }

    return 0;
}

void run_tests(void) {
    const char *tls_tests[] = {
        "abba[mnop]qrst",
        "abcd[bddb]xyyx",
        "aaaa[qwer]tyui",
        "ioxxoj[asdfgh]zxcvbn"
    };
    int tls_expected[] = {1, 0, 0, 1};

    const char *ssl_tests[] = {
        "aba[bab]xyz",
        "xyx[xyx]xyx",
        "aaa[kek]eke",
        "zazbz[bzb]cdb"
    };
    int ssl_expected[] = {1, 0, 1, 1};

    int i, res;

    cprintf("\r\nRUNNING TLS TESTS...\r\n");
    for (i = 0; i < 4; i++) {
        res = supports_tls(tls_tests[i]);
        cprintf("%s: %s\r\n", tls_tests[i], (res == tls_expected[i]) ? "PASS" : "FAIL");
    }

    cprintf("\r\nRUNNING SSL TESTS...\r\n");
    for (i = 0; i < sizeof(ssl_tests) / sizeof(ssl_tests[0]); i++) {
        res = supports_ssl(ssl_tests[i]);
        cprintf("%s: %s\r\n", ssl_tests[i], (res == ssl_expected[i]) ? "PASS" : "FAIL");
    }
}

int main(void) {
    bgcolor(COLOR_BLACK);
    bordercolor(COLOR_GRAY3);
    textcolor(COLOR_GREEN);
    clrscr();
    gotoxy(0, 0);

    cprintf("ADVENT OF CODE 2016 - DAY 7\r\n");
    cprintf("INTERNET PROTOCOL VERSION 7\r\n");
    cprintf("============================\r\n");

    run_tests();

    cprintf("\r\nPRESS ENTER TO EXIT.\r\n");
    cgetc();

    return 0;
}
