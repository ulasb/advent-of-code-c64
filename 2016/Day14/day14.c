#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"

static const char HEX_DIGITS[] = "0123456789abcdef";

/* Global buffers to save stack space */
MD5_CTX global_ctx;
char hex_buffer[34];
uint8 digest[16];

/* Window Management */
#define WINDOW_SIZE 1001
uint8 window_triplet[WINDOW_SIZE];
unsigned int window_quintets[WINDOW_SIZE];
int quintet_counts[16];

/* Helper functions */
static uint8 hex_char_to_val(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    return c - 'a' + 10;
}

static void add_quintets_to_counts(unsigned int q) {
    int i;
    for (i = 0; i < 16; i++) {
        if (q & (1 << i)) quintet_counts[i]++;
    }
}

static void remove_quintets_from_counts(unsigned int q) {
    int i;
    for (i = 0; i < 16; i++) {
        if (q & (1 << i)) quintet_counts[i]--;
    }
}

/* Function Prototypes */
void bin_to_hex(const uint8 *bin, char *hex);
void analyze_hash(const char *hex, uint8 *triplet, unsigned int *quintets);
void get_hash(const char *salt, int salt_len, unsigned long index, int stretched, char *out_hex);
void solve(const char *salt, int stretched, int num_keys);

int main(void) {
    bgcolor(COLOR_BLUE);
    bordercolor(COLOR_LIGHTBLUE);
    textcolor(COLOR_WHITE);
    clrscr();

    cprintf("ADVENT OF CODE 2016 - DAY 14\r\n");
    cprintf("==============================\r\n");

    /* Default salt from puzzle or test */
    solve("ihaygndm", 0, 64); // Actual salt Part 1

    cprintf("\r\nPRESS ANY KEY TO EXIT.\r\n");
    cgetc();

    return 0;
}

void bin_to_hex(const uint8 *bin, char *hex) {
    int i;
    for (i = 0; i < 16; i++) {
        hex[i*2] = HEX_DIGITS[bin[i] >> 4];
        hex[i*2 + 1] = HEX_DIGITS[bin[i] & 0x0F];
    }
    hex[32] = 0;
}

void analyze_hash(const char *hex, uint8 *triplet, unsigned int *quintets) {
    int i;
    int count = 1;
    char last_char = hex[0];
    
    *triplet = 255; /* None */
    *quintets = 0;
    
    for (i = 1; i < 32; i++) {
        if (hex[i] == last_char) {
            count++;
            if (count == 3 && *triplet == 255) {
                /* Found first triplet */
                *triplet = hex_char_to_val(last_char);
            }
            if (count == 5) {
                /* Found quintet */
                *quintets |= (1 << hex_char_to_val(last_char));
            }
        } else {
            count = 1;
            last_char = hex[i];
        }
    }
}

void get_hash(const char *salt, int salt_len, unsigned long index, int stretched, char *out_hex) {
    char input[32];
    int len;
    int i;
    
    memcpy(input, salt, salt_len);
    ltoa(index, input + salt_len, 10);
    len = strlen(input);
    
    md5_init(&global_ctx);
    md5_update(&global_ctx, (const uint8*)input, len);
    md5_final(digest, &global_ctx);
    bin_to_hex(digest, out_hex);
    
    if (stretched) {
        for (i = 0; i < 2016; i++) {
            md5_init(&global_ctx);
            md5_update(&global_ctx, (const uint8*)out_hex, 32);
            md5_final(digest, &global_ctx);
            bin_to_hex(digest, out_hex);
        }
    }
}

void solve(const char *salt, int stretched, int num_keys) {
    int keys_found = 0;
    unsigned long index = 0;
    int i;
    uint8 t;
    unsigned int q;
    int head = 0; /* Circular buffer head */
    int salt_len = strlen(salt);

    cprintf("Salt: %s\r\n", salt);
    cprintf("Mode: %s\r\n", stretched ? "Stretched" : "Normal");
    
    /* Initialize quintet_counts */
    for (i = 0; i < 16; i++) quintet_counts[i] = 0;

    /* Fill initial window */
    cprintf("Initializing window...\r\n");
    for (index = 0; index < WINDOW_SIZE; index++) {
        get_hash(salt, salt_len, index, stretched, hex_buffer);
        analyze_hash(hex_buffer, &t, &q);
        window_triplet[index] = t;
        window_quintets[index] = q;
        
        /* Add to counts if in lookahead (index 1 to 1000) */
        if (index > 0) {
            add_quintets_to_counts(q);
        }
        
        if ((index & 0x3F) == 0) cprintf("\rPos: %lu", index);
    }
    cprintf("\rWindow ready. Searching...\r\n");

    index = 0;
    while (keys_found < num_keys) {
        /* Current hash is at 'head' */
        uint8 tri = window_triplet[head];
        
        if (tri != 255) {
            if (quintet_counts[tri] > 0) {
                keys_found++;
                cprintf("[%2d/64] Key at %lu   \r\n", keys_found, index);
            }
        }

        /* Slide Window */
        /* 1. Remove hash at (head + 1) from quintet_counts */
        {
            int next_head = (head + 1) % WINDOW_SIZE;
            remove_quintets_from_counts(window_quintets[next_head]);
        }

        /* 2. Generate new hash and replace 'head' */
        get_hash(salt, salt_len, index + WINDOW_SIZE, stretched, hex_buffer);
        analyze_hash(hex_buffer, &t, &q);
        window_triplet[head] = t;
        window_quintets[head] = q;

        /* 3. Add new hash to quintet_counts */
        add_quintets_to_counts(q);

        /* 4. Move head */
        head = (head + 1) % WINDOW_SIZE;
        index++;

        if ((index & 0x3F) == 0) {
            cprintf("\rIdx: %lu", index);
            if (kbhit()) if (cgetc() == 27) break;
        }
    }

    cprintf("\r\nDONE! 64th key index: %lu\r\n", index - 1);
}
