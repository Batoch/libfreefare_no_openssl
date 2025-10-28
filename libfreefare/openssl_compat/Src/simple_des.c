#include "simple_des.h"

// Permuted Choice 1 table
static const uint8_t PC1[56] = {
 57,49,41,33,25,17,9, 1,58,50,42,34,26,18,
 10,2,59,51,43,35,27,19,11,3,60,52,44,36,
 63,55,47,39,31,23,15,7,62,54,46,38,30,22,
 14,6,61,53,45,37,29,21,13,5,28,20,12,4
};
// Permuted Choice 2 table
static const uint8_t PC2[48] = {
 14,17,11,24,1,5,3,28,15,6,21,10,23,19,12,4,
 26,8,16,7,27,20,13,2,41,52,31,37,47,55,30,40,
 51,45,33,48,44,49,39,56,34,53,46,42,50,36,29,32
};
// Initial Permutation table
static const uint8_t IP[64] = {
 58,50,42,34,26,18,10,2,60,52,44,36,28,20,12,4,
 62,54,46,38,30,22,14,6,64,56,48,40,32,24,16,8,
 57,49,41,33,25,17,9,1,59,51,43,35,27,19,11,3,
 61,53,45,37,29,21,13,5,63,55,47,39,31,23,15,7
};
// Final Permutation table
static const uint8_t FP[64] = {
 40,8,48,16,56,24,64,32,39,7,47,15,55,23,63,31,
 38,6,46,14,54,22,62,30,37,5,45,13,53,21,61,29,
 36,4,44,12,52,20,60,28,35,3,43,11,51,19,59,27,
 34,2,42,10,50,18,58,26,33,1,41,9,49,17,57,25
};
// Expansion table
static const uint8_t E[48] = {
 32,1,2,3,4,5,4,5,6,7,8,9,8,9,10,11,12,13,
 12,13,14,15,16,17,16,17,18,19,20,21,20,21,22,23,24,25,
 24,25,26,27,28,29,28,29,30,31,32,1
};
// S-boxes
static const uint8_t S[8][64] = {
  {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13},
  {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9},
  {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12},
  {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14},
  {2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3},
  {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13},
  {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12},
  {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
};
// Permutation table
static const uint8_t P[32] = {
 16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,
 2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25
};

// Left rotation
static uint32_t rol28(uint32_t x, int n) {
    return ((x << n) | (x >> (28 - n))) & 0x0FFFFFFF;
}

// Permute bits in src according to table, n bits
static uint64_t permute(uint64_t src, const uint8_t *table, int n, int bitwidth) {
    uint64_t dst = 0;
    for (int i = 0; i < n; i++) {
        dst <<= 1;
        dst |= (src >> (bitwidth - table[i])) & 1;
    }
    return dst;
}

// Generate 16 subkeys
static void des_key_schedule(const uint8_t *key, uint64_t subkeys[16]) {
    uint64_t k = 0;
    for (int i = 0; i < 8; i++) k = (k << 8) | key[i];
    k = permute(k, PC1, 56, 64);
    uint32_t c = (k >> 28) & 0x0FFFFFFF, d = k & 0x0FFFFFFF;
    static const int shifts[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};
    for (int i = 0; i < 16; i++) {
        c = rol28(c, shifts[i]);
        d = rol28(d, shifts[i]);
        uint64_t cd = ((uint64_t)c << 28) | d;
    subkeys[i] = permute(cd, PC2, 48, 56);
    }
}

// Feistel function
static uint32_t feistel(uint32_t r, uint64_t k) {
    // Expansion E
    uint64_t e = 0;
    for (int i = 0; i < 48; i++) {
        e <<= 1;
        e |= (r >> (32 - E[i])) & 1;
    }
    e ^= k;
    // S-boxes
    uint32_t sbox_out = 0;
    for (int i = 0; i < 8; i++) {
        int sixbits = (e >> (42 - 6 * i)) & 0x3F;
        int row = ((sixbits & 0x20) >> 4) | (sixbits & 0x01);
        int col = (sixbits >> 1) & 0x0F;
        sbox_out = (sbox_out << 4) | S[i][row * 16 + col];
    }
    // Permutation P
    uint32_t p = 0;
    for (int i = 0; i < 32; i++) {
        p <<= 1;
        p |= (sbox_out >> (32 - P[i])) & 1;
    }
    return p;
}

void simple_des_ecb_encrypt(const uint8_t *input, uint8_t *output, const uint8_t *key, int enc) {
    uint64_t subkeys[16];
    des_key_schedule(key, subkeys);
    if (!enc) {
        // Reverse subkeys for decryption
        for (int i = 0; i < 8; i++) {
            uint64_t tmp = subkeys[i];
            subkeys[i] = subkeys[15 - i];
            subkeys[15 - i] = tmp;
        }
    }
    uint64_t block = 0;
    for (int i = 0; i < 8; i++) block = (block << 8) | input[i];
    block = permute(block, IP, 64, 64);
    uint32_t l = (block >> 32) & 0xFFFFFFFF, r = block & 0xFFFFFFFF;
    for (int i = 0; i < 16; i++) {
        uint32_t tmp = r;
        r = l ^ feistel(r, subkeys[i]);
        l = tmp;
    }
    // Swap L and R before final permutation
    block = ((uint64_t)r << 32) | l;
    block = permute(block, FP, 64, 64);
    for (int i = 0; i < 8; i++) {
        output[i] = (block >> (56 - 8 * i)) & 0xFF;
    }
}

