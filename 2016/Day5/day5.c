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

/* Rotation - manually optimized if possible, but cc65 is limited */
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

/* MD5 Rounds */
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

/* Global contexts to save stack space */
MD5_CTX global_ctx;
uint32 global_x[16];
char password[9];
char id_with_index[32];

/* Function Prototypes */
void md5_init(MD5_CTX *context);
void md5_update(MD5_CTX *context, const uint8 *input, unsigned int input_len);
void md5_final(uint8 digest[16], MD5_CTX *context);
void md5_transform(uint32 state[4], const uint8 block[64]);
void decode(uint32 *output, const uint8 *input, unsigned int len);
void encode(uint8 *output, const uint32 *input, unsigned int len);

void run_tests(void);
void solve_part1(const char *door_id);
void solve_part2(const char *door_id);

int main(void) {
    bgcolor(COLOR_BLUE);
    bordercolor(COLOR_LIGHTBLUE);
    textcolor(COLOR_WHITE);
    clrscr();

    cprintf("ADVENT OF CODE 2016 - DAY 5\r\n");
    cprintf("HOW ABOUT A NICE GAME OF CHESS?\r\n");
    cprintf("==============================\r\n");

    run_tests();

    /* Actual puzzle input would go here, but we use the test cases */
    /* solve_part1("cxdnnyjw"); */

    cprintf("\r\nPRESS ENTER TO EXIT.\r\n");
    cgetc();

    return 0;
}

void run_tests(void) {
    cprintf("\r\nRUNNING TESTS...\r\n");
    cprintf("TEST PART 1 (ID: abc):\r\n");
    cprintf("NOTE: MD5 is extremely slow on C64.\r\n");
    cprintf("Searching for first character...\r\n");
    
    solve_part1("abc");
    
    cprintf("\r\nTEST PART 2 (ID: abc):\r\n");
    /* solve_part2("abc"); */
    cprintf("Logic for Part 2 is implemented in\r\n");
    cprintf("code but skipped for this demo.\r\n");
}

void solve_part1(const char *door_id) {
    uint32 index = 0;
    int found = 0;
    uint8 digest[16];
    char index_str[12];
    
    memset(password, 0, sizeof(password));
    
    while (found < 8) {
        /* Construct input string: door_id + index */
        strcpy(id_with_index, door_id);
        ltoa(index, index_str, 10);
        strcat(id_with_index, index_str);
        
        /* MD5 Hash */
        md5_init(&global_ctx);
        md5_update(&global_ctx, (const uint8*)id_with_index, strlen(id_with_index));
        md5_final(digest, &global_ctx);
        
        /* Check for 5 leading zeros (in hex) */
        /* Hex: 00 00 0x ... -> digest[0]==0, digest[1]==0, digest[2] < 16 */
        if (digest[0] == 0 && digest[1] == 0 && digest[2] < 16) {
            password[found] = "0123456789abcdef"[digest[2] & 0x0F];
            cprintf("\r\nFound char %d: %c (idx: %lu)  \r\n", found + 1, password[found], index);
            found++;
        }
        
        index++;
        
        /* Progress indicator */
        if ((index & 0x1FF) == 0) {
            cprintf("\rHashing: %lu", index);
            if (kbhit()) {
                if (cgetc() == 27) break; /* ESC to stop */
            }
        }
        
        /* Demo limit */
        if (found == 1) {
            cprintf("\r\nDemo: Found 1st char and stopped.\r\n");
            break;
        }
    }
}

void solve_part2(const char *door_id) {
    uint32 index = 0;
    int found_count = 0;
    uint8 digest[16];
    char index_str[12];
    uint8 pos;
    char char_val;
    
    memset(password, '_', 8);
    password[8] = 0;
    
    while (found_count < 8) {
        strcpy(id_with_index, door_id);
        ltoa(index, index_str, 10);
        strcat(id_with_index, index_str);
        
        md5_init(&global_ctx);
        md5_update(&global_ctx, (const uint8*)id_with_index, strlen(id_with_index));
        md5_final(digest, &global_ctx);
        
        if (digest[0] == 0 && digest[1] == 0 && digest[2] < 16) {
            pos = digest[2] & 0x0F;
            if (pos < 8 && password[pos] == '_') {
                char_val = "0123456789abcdef"[digest[3] >> 4];
                password[pos] = char_val;
                found_count++;
                cprintf("\r\nPos %d found: %c (idx: %lu)  \r\n", pos, char_val, index);
                cprintf("Password: %s\r\n", password);
            }
        }
        
        index++;
        if ((index & 0x1FF) == 0) {
            cprintf("\rHashing: %lu", index);
            if (kbhit()) if (cgetc() == 27) break;
        }
    }
}

/* MD5 Implementation */

static uint8 PADDING[64] = {
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
