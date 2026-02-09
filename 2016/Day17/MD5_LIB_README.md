# C64 MD5 Library (cc65)

A robust, verified MD5 implementation for the Commodore 64, built using the `cc65` compiler suite.

## Features
- Standard MD5 (RFC 1321) implementation.
- Optimized for the 6502 architecture pitfalls.
- Verified against standard ASCII test vectors.
- Includes a test suite and debugging tools.

## The "Tricky Stuff" (C64/cc65 Specifics)

Developing 32-bit cryptographic algorithms for an 8-bit CPU with `cc65` requires special attention to several areas:

### 1. PETSCII vs. ASCII Encoding
The Commodore 64 uses **PETSCII** by default for string literals. In PETSCII, `'a'` is `0x41`, but standard MD5 test vectors expect **ASCII** where `'a'` is `0x61`. 
- **Solution**: For standard testing, use explicit hex byte arrays (e.g., `{0x61, 0x62, 0x63}`) rather than string literals to ensure the bytes match the MD5 specification.

### 2. 32-bit Arithmetic Complexity
`cc65` can struggle with deep expression trees involving `uint32_t`. Complex macros often fail because the compiler runs out of registers or generates incorrect sign-extension assembly.
- **Solution**: The internal MD5 round macros (`FF`, `GG`, `HH`, `II`) are broken down into simple, sequential C statements. Each addition and bitwise operation is explicitly masked with `& 0xFFFFFFFFUL`.

### 3. Bitwise NOT (~) and Promotion
In `cc65`, the `~` operator on a 32-bit value can sometimes be treated shiftingly if the promotion rules aren't strictly enforced.
- **Solution**: Every logical operation (F, G, H, I) uses explicit `(uint32_t)` casts before the NOT operator and re-masks the result.

### 4. Endianness (Decode/Encode)
MD5 is a little-endian algorithm. While the 6502 is naturally little-endian for 16-bit values, `cc65`'s handling of 32-bit types can vary depending on pointer casting.
- **Solution**: Explicit `Decode` and `Encode` functions are used to manually unpack bytes into 32-bit words, ensuring consistency regardless of compiler-specific memory layout.

### 5. Compiler Infrastructure
We recommend building without high optimization (`-O`) if you encounter stability issues with 32-bit math, although the current implementation is designed to be safe with or without it.

## Build Instructions

Requires `cc65` installed and in your path.
=======
- Handles inputs larger than 64KB (uses 32-bit length).
- Memory-efficient: `const` padding saved in ROM/data.
- Conditional debugging system via preprocessor macros.

## The "Tricky Stuff" (C64/cc65 Specifics)

### 1. 32-bit Arithmetic & Type Promotion
`cc65` uses 16-bit `int` and `unsigned int`. The MD5 algorithm requires 32-bit precision for state, bit-counts, and buffer management. 
- **Solution**: All internal state and length variables are defined as `uint32_t` (from `<stdint.h>`). We use explicit `& 0xFFFFFFFFUL` masking to ensure correct 32-bit truncation on the 8-bit CPU.

### 2. Expression Complexity
Deep expression trees can confuse the `cc65` code generator or exceed its register capacity.
- **Solution**: Round transformations are broken down into simple, atomic C statements to ensure stable code generation.

### 3. PETSCII vs. ASCII
The C64 uses PETSCII locally. Standard MD5 vectors are ASCII-based.
- **Solution**: Test cases in `main.c` use explicit ASCII hex byte arrays to guarantee correctness against the MD5 spec.

### 4. Debugging & Code Size
Logging with `printf` and internal consistency tests consume significant space on the 1MHz/64KB C64.
- **Solution**: A macro-based logging system (`MD5_DEBUG`) allows zero-overhead production builds. 

## Build Instructions

Requires `cc65` installed.

```bash
make clean
make
```

## Usage

```c
#include "md5.h"

MD5_CTX ctx;
unsigned char digest[16];
unsigned char data[] = { 0x61, 0x62, 0x63 }; // "abc"

MD5Init(&ctx);
MD5Update(&ctx, data, 3);
MD5Final(digest, &ctx);
```
