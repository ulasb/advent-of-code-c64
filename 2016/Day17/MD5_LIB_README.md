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

```bash
make clean
make
```

### PETSCII vs. ASCII when building with `cc65`

String literals compiled with `cc65` are PETSCII on the C64. The MD5 tests and
Advent of Code solver expect ASCII bytes when computing hashes. The source
provides conditional conversion from PETSCII to ASCII, but some toolchains or
build environments may not define `__CC65__` automatically.

If you see mismatched hashes or failing tests on the C64 build, build with the
explicit conversion enabled:

```bash
cl65 -t c64 -DFORCE_PETSCII_TO_ASCII -O -o day17.prg day17.c md5.c
```

Or update the `Makefile` to add `-DFORCE_PETSCII_TO_ASCII` to the `CFLAGS`.

Alternatively, when testing on the host, you can reproduce PETSCII literal
encoding by building the host simulator which defines `SIMULATE_PETSCII`:

```bash
gcc -std=c99 -Wall -O2 -DSIMULATE_PETSCII day17.c md5.c -o day17_host_sim
./day17_host_sim
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
