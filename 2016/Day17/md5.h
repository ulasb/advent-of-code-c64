#ifndef MD5_H
#define MD5_H

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
void md5_init(MD5_CTX *context);
void md5_update(MD5_CTX *context, const uint8 *input, unsigned int input_len);
void md5_final(uint8 digest[16], MD5_CTX *context);
void md5_transform(uint32 state[4], const uint8 block[64]);

/* Internal helpers used by MD5 implementation */
void decode(uint32 *output, const uint8 *input, unsigned int len);
void encode(uint8 *output, const uint32 *input, unsigned int len);

#endif /* MD5_H */
