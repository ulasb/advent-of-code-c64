#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* MD5 constants and types */
typedef unsigned long uint32;
typedef unsigned char uint8;

/* MD5 context */
typedef struct {
    uint32 state[4];
    uint32 count[2];
    uint8 buffer[64];
} MD5_CTX;

/* MD5 basic functions */
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

#define FF(a, b, c, d, x, s, ac) { \
    (a) += F((b), (c), (d)) + (x) + (uint32)(ac); \
    (a) = ROTATE_LEFT((a), (s)); \
    (a) += (b); \
}
#define GG(a, b, c, d, x, s, ac) { \
    (a) += G((b), (c), (d)) + (x) + (uint32)(ac); \
    (a) = ROTATE_LEFT((a), (s)); \
    (a) += (b); \
}
#define HH(a, b, c, d, x, s, ac) { \
    (a) += H((b), (c), (d)) + (x) + (uint32)(ac); \
    (a) = ROTATE_LEFT((a), (s)); \
    (a) += (b); \
}
#define II(a, b, c, d, x, s, ac) { \
    (a) += I((b), (c), (d)) + (x) + (uint32)(ac); \
    (a) = ROTATE_LEFT((a), (s)); \
    (a) += (b); \
}

static const char HEX_DIGITS[] = "0123456789abcdef";

/* Global buffers to save stack space */
MD5_CTX global_ctx;
uint32 global_x[16];
char hex_buffer[34];
uint8 digest[16];

/* Window Management */
#define WINDOW_SIZE 1001
uint8 window_triplet[WINDOW_SIZE];
unsigned int window_quintets[WINDOW_SIZE];
int quintet_counts[16];
unsigned long current_index = 0;

/* Function Prototypes */
void md5_init(MD5_CTX *context);
void md5_update(MD5_CTX *context, const uint8 *input, unsigned int input_len);
void md5_final(uint8 digest[16], MD5_CTX *context);
void md5_transform(uint32 state[4], const uint8 block[64]);
void decode(uint32 *output, const uint8 *input, unsigned int len);
void encode(uint8 *output, const uint32 *input, unsigned int len);
void bin_to_hex(const uint8 *bin, char *hex);

void analyze_hash(const char *hex, uint8 *triplet, unsigned int *quintets);
void get_hash(const char *salt, unsigned long index, int stretched, char *out_hex);

void solve(const char *salt, int stretched, int num_keys);

int main(void) {
    bgcolor(COLOR_BLUE);
    bordercolor(COLOR_LIGHTBLUE);
    textcolor(COLOR_WHITE);
    clrscr();

    cprintf("ADVENT OF CODE 2016 - DAY 14\r\n");
    cprintf("==============================\r\n");

    /* Default salt from puzzle or test */
    /* solve("abc", 0, 64); // Test salt Part 1 */
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
                if (last_char >= '0' && last_char <= '9') *triplet = last_char - '0';
                else *triplet = last_char - 'a' + 10;
            }
            if (count == 5) {
                /* Found quintet */
                uint8 char_val;
                if (last_char >= '0' && last_char <= '9') char_val = last_char - '0';
                else char_val = last_char - 'a' + 10;
                *quintets |= (1 << char_val);
            }
        } else {
            count = 1;
            last_char = hex[i];
        }
    }
}

void get_hash(const char *salt, unsigned long index, int stretched, char *out_hex) {
    char input[32];
    int len;
    int i;
    
    strcpy(input, salt);
    ltoa(index, input + strlen(salt), 10);
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

    cprintf("Salt: %s\r\n", salt);
    cprintf("Mode: %s\r\n", stretched ? "Stretched" : "Normal");
    
    /* Initialize quintet_counts */
    for (i = 0; i < 16; i++) quintet_counts[i] = 0;

    /* Fill initial window */
    cprintf("Initializing window...\r\n");
    for (index = 0; index < WINDOW_SIZE; index++) {
        get_hash(salt, index, stretched, hex_buffer);
        analyze_hash(hex_buffer, &t, &q);
        window_triplet[index] = t;
        window_quintets[index] = q;
        
        /* Add to counts if in lookahead (index 1 to 1000) */
        if (index > 0) {
            for (i = 0; i < 16; i++) {
                if (q & (1 << i)) quintet_counts[i]++;
            }
        }
        
        if ((index & 0x3F) == 0) cprintf("\rPos: %lu", index);
    }
    cprintf("\rWindow ready. Searching...\r\n");

    index = 0;
    while (keys_found < num_keys) {
        /* Current hash is at index in the window */
        /* But window is circular, so it's at 'head' */
        uint8 tri = window_triplet[head];
        
        if (tri != 255) {
            if (quintet_counts[tri] > 0) {
                keys_found++;
                cprintf("[%2d/64] Key at %lu   \r\n", keys_found, index);
            }
        }

        /* Slide Window */
        /* 1. Remove hash at (head + 1) from quintet_counts as it's about to be current */
        {
            int next_head = (head + 1) % WINDOW_SIZE;
            unsigned int next_q = window_quintets[next_head];
            for (i = 0; i < 16; i++) {
                if (next_q & (1 << i)) quintet_counts[i]--;
            }
        }

        /* 2. Generate new hash at index + WINDOW_SIZE and replace current 'head' */
        get_hash(salt, index + WINDOW_SIZE, stretched, hex_buffer);
        analyze_hash(hex_buffer, &t, &q);
        window_triplet[head] = t;
        window_quintets[head] = q;

        /* 3. Add new hash to quintet_counts */
        for (i = 0; i < 16; i++) {
            if (q & (1 << i)) quintet_counts[i]++;
        }

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

/* MD5 Implementation (copied from day5.c) */

static const uint8 PADDING[64] = {
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void md5_init(MD5_CTX *context) {
    context->count[0] = context->count[1] = 0;
    context->state[0] = 0x67452301;
    context->state[1] = 0xefcdab89;
    context->state[2] = 0x98badcfe;
    context->state[3] = 0x10325476;
}

void md5_update(MD5_CTX *context, const uint8 *input, unsigned int input_len) {
    unsigned int i, index, part_len;

    index = (unsigned int)((context->count[0] >> 3) & 0x3F);
    if ((context->count[0] += ((uint32)input_len << 3)) < ((uint32)input_len << 3))
        context->count[1]++;
    context->count[1] += ((uint32)input_len >> 29);

    part_len = 64 - index;

    if (input_len >= part_len) {
        memcpy(&context->buffer[index], input, part_len);
        md5_transform(context->state, context->buffer);
        for (i = part_len; i + 63 < input_len; i += 64)
            md5_transform(context->state, &input[i]);
        index = 0;
    } else {
        i = 0;
    }

    memcpy(&context->buffer[index], &input[i], input_len - i);
}

void md5_final(uint8 digest[16], MD5_CTX *context) {
    uint8 bits[8];
    unsigned int index, pad_len;

    encode(bits, context->count, 8);
    index = (unsigned int)((context->count[0] >> 3) & 0x3F);
    pad_len = (index < 56) ? (56 - index) : (120 - index);
    md5_update(context, PADDING, pad_len);
    md5_update(context, bits, 8);
    encode(digest, context->state, 16);
    /* Clean up sensitive information */
    memset(context, 0, sizeof(*context));
}

void md5_transform(uint32 state[4], const uint8 block[64]) {
    uint32 a = state[0], b = state[1], c = state[2], d = state[3];
    
    decode(global_x, block, 64);

    /* Round 1 */
    FF(a, b, c, d, global_x[ 0], 7, 0xd76aa478);
    FF(d, a, b, c, global_x[ 1], 12, 0xe8c7b756);
    FF(c, d, a, b, global_x[ 2], 17, 0x242070db);
    FF(b, c, d, a, global_x[ 3], 22, 0xc1bdceee);
    FF(a, b, c, d, global_x[ 4], 7, 0xf57c0faf);
    FF(d, a, b, c, global_x[ 5], 12, 0x4787c62a);
    FF(c, d, a, b, global_x[ 6], 17, 0xa8304613);
    FF(b, c, d, a, global_x[ 7], 22, 0xfd469501);
    FF(a, b, c, d, global_x[ 8], 7, 0x698098d8);
    FF(d, a, b, c, global_x[ 9], 12, 0x8b44f7af);
    FF(c, d, a, b, global_x[10], 17, 0xffff5bb1);
    FF(b, c, d, a, global_x[11], 22, 0x895cd7be);
    FF(a, b, c, d, global_x[12], 7, 0x6b901122);
    FF(d, a, b, c, global_x[13], 12, 0xfd987193);
    FF(c, d, a, b, global_x[14], 17, 0xa679438e);
    FF(b, c, d, a, global_x[15], 22, 0x49b40821);

    /* Round 2 */
    GG(a, b, c, d, global_x[ 1], 5, 0xf61e2562);
    GG(d, a, b, c, global_x[ 6], 9, 0xc040b340);
    GG(c, d, a, b, global_x[11], 14, 0x265e5a51);
    GG(b, c, d, a, global_x[ 0], 20, 0xe9b6c7aa);
    GG(a, b, c, d, global_x[ 5], 5, 0xd62f105d);
    GG(d, a, b, c, global_x[10], 9, 0x02441453);
    GG(c, d, a, b, global_x[15], 14, 0xd8a1e681);
    GG(b, c, d, a, global_x[ 4], 20, 0xe7d3fbc8);
    GG(a, b, c, d, global_x[ 9], 5, 0x21e1cde6);
    GG(d, a, b, c, global_x[14], 9, 0xc33707d6);
    GG(c, d, a, b, global_x[ 3], 14, 0xf4d50d87);
    GG(b, c, d, a, global_x[ 8], 20, 0x455a14ed);
    GG(a, b, c, d, global_x[13], 5, 0xa9e3e905);
    GG(d, a, b, c, global_x[ 2], 9, 0xfcefa3f8);
    GG(c, d, a, b, global_x[ 7], 14, 0x676f02d9);
    GG(b, c, d, a, global_x[12], 20, 0x8d2a4c8a);

    /* Round 3 */
    HH(a, b, c, d, global_x[ 5], 4, 0xfffa3942);
    HH(d, a, b, c, global_x[ 8], 11, 0x8771f681);
    HH(c, d, a, b, global_x[11], 16, 0x6d9d6122);
    HH(b, c, d, a, global_x[14], 23, 0xfde5380c);
    HH(a, b, c, d, global_x[ 1], 4, 0xa4beea44);
    HH(d, a, b, c, global_x[ 4], 11, 0x4bdecfa9);
    HH(c, d, a, b, global_x[ 7], 16, 0xf6bb4b60);
    HH(b, c, d, a, global_x[10], 23, 0xbebfbc70);
    HH(a, b, c, d, global_x[13], 4, 0x289b7ec6);
    HH(d, a, b, c, global_x[ 0], 11, 0xeaa127fa);
    HH(c, d, a, b, global_x[ 3], 16, 0xd4ef3085);
    HH(b, c, d, a, global_x[ 6], 23, 0x04881d05);
    HH(a, b, c, d, global_x[ 9], 4, 0xd9d4d039);
    HH(d, a, b, c, global_x[12], 11, 0xe6db99e5);
    HH(c, d, a, b, global_x[15], 16, 0x1fa27cf8);
    HH(b, c, d, a, global_x[ 2], 23, 0xc4ac5665);

    /* Round 4 */
    II(a, b, c, d, global_x[ 0], 6, 0xf4292244);
    II(d, a, b, c, global_x[ 7], 10, 0x432aff97);
    II(c, d, a, b, global_x[14], 15, 0xab9423a7);
    II(b, c, d, a, global_x[ 5], 21, 0xfc93a039);
    II(a, b, c, d, global_x[12], 6, 0x655b59c3);
    II(d, a, b, c, global_x[ 3], 10, 0x8f0ccc92);
    II(c, d, a, b, global_x[10], 15, 0xffeff47d);
    II(b, c, d, a, global_x[ 1], 21, 0x85845dd1);
    II(a, b, c, d, global_x[ 8], 6, 0x6fa87e4f);
    II(d, a, b, c, global_x[15], 10, 0xfe2ce6e0);
    II(c, d, a, b, global_x[ 6], 15, 0xa3014314);
    II(b, c, d, a, global_x[13], 21, 0x4e0811a1);
    II(a, b, c, d, global_x[ 4], 6, 0xf7537e82);
    II(d, a, b, c, global_x[11], 10, 0xbd3af235);
    II(c, d, a, b, global_x[ 2], 15, 0x2ad7d2bb);
    II(b, c, d, a, global_x[ 9], 21, 0xeb86d391);

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;

    memset(global_x, 0, sizeof(global_x));
}

void encode(uint8 *output, const uint32 *input, unsigned int len) {
    unsigned int i, j;
    for (i = 0, j = 0; j < len; i++, j += 4) {
        output[j] = (uint8)(input[i] & 0xff);
        output[j + 1] = (uint8)((input[i] >> 8) & 0xff);
        output[j + 2] = (uint8)((input[i] >> 16) & 0xff);
        output[j + 3] = (uint8)((input[i] >> 24) & 0xff);
    }
}

void decode(uint32 *output, const uint8 *input, unsigned int len) {
    unsigned int i, j;
    for (i = 0, j = 0; j < len; i++, j += 4)
        output[i] = ((uint32)input[j]) | (((uint32)input[j + 1]) << 8) |
                    (((uint32)input[j + 2]) << 16) | (((uint32)input[j + 3]) << 24);
}
