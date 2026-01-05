# Advent of Code 2015 - Day 4 (C64 Edition)

## Why No Code Implementation

After careful consideration, there is **no practical C64 implementation** provided for Advent of Code 2015 Day 4. The computational requirements of this problem make it fundamentally incompatible with Commodore 64 hardware limitations.

## The Problem: "The Ideal Stocking Stuffer"

Santa needs help mining AdventCoins using a proof-of-work system similar to Adam Back's Hashcash. The goal is to find MD5 hashes that start with a specific number of leading zeros.

### Part 1
Find the lowest positive number that, when appended to the secret key "bgvyzdsv", produces an MD5 hash starting with at least **five zeros**.

### Part 2
Find the lowest positive number that, when appended to the secret key "bgvyzdsv", produces an MD5 hash starting with at least **six zeros**.

### Example
- Input: "abcdef" + some number
- If the number is 609043, then "abcdef609043" produces MD5 hash "000001dbbfa..."
- The hash starts with 5 zeros, so 609043 is the answer for 5 zeros

## Technical Challenges on C64

### Computational Complexity
- **MD5 Algorithm**: Requires processing 512-bit blocks with complex mathematical operations
- **Brute Force Search**: Must try numbers sequentially until finding one that produces the required hash pattern
- **Scale**: Part 1 requires ~250,000 attempts, Part 2 requires ~1,000,000 attempts

### C64 Hardware Limitations
- **Processor**: 1MHz 6510 CPU (roughly 1 million instructions per second)
- **Memory**: 64KB total RAM, with significant overhead for OS and program
- **cc65 Compiler**: Limited optimization, no advanced math libraries
- **Storage**: No persistent storage for intermediate results

### Performance Estimate
- **Modern Computer**: Part 1 takes seconds, Part 2 could take minutes
- **C64 Estimate**: Part 1 would likely take hours, Part 2 could take days or weeks
- **Memory Usage**: MD5 state alone requires managing 128+ bytes of working memory

## What a Proper Solution Would Require

### Algorithm Overview
1. **MD5 Implementation**:
   ```c
   // Message padding
   append "1" bit + padding zeros
   append 64-bit message length

   // Process in 512-bit blocks
   for each 512-bit block:
       divide into sixteen 32-bit words
       perform 4 rounds of 16 operations each
       update hash state (A, B, C, D)
   ```

2. **Brute Force Search**:
   ```c
   for (int i = 0; ; i++) {
       char buffer[256];
       sprintf(buffer, "%s%d", secret_key, i);

       unsigned char hash[16];
       md5_hash(buffer, hash);

       char hex[33];
       bytes_to_hex(hash, hex, 16);

       if (strncmp(hex, "00000", 5) == 0) {
           return i;
       }
   }
   ```

### MD5 Functions Required
- **Padding**: Add '1' bit and length padding
- **Bit Operations**: Left rotations, bitwise AND/OR/XOR
- **State Management**: Four 32-bit registers (A, B, C, D)
- **Four Rounds**: Different functions (F, G, H, I) with sine-based constants


## Alternative Approaches Considered

1. **Precomputed Lookup Table**: Too large for C64 memory
2. **Simplified Hash Function**: Would not match MD5 specification
3. **Assembly Implementation**: Possible but still too slow
4. **Hardware Acceleration**: Not available on stock C64

## Files in This Directory

- `README.md` - This explanation document

## Conclusion

This problem represents the limits of what retro computing can achieve. While the C64 was revolutionary in its time, problems requiring millions of cryptographic operations exceed its practical capabilities. This serves as a reminder of how far computing power has advanced since 1982.

For a proper solution, see modern implementations in languages like Python, JavaScript, or C++ that can complete this problem in under a minute on contemporary hardware. I have a Python solution [here](https://github.com/ulasb/Python-Algorithms/blob/main/advent-of-code/2015/day_4/hash.py).
