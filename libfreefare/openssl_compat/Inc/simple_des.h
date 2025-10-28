#ifndef SIMPLE_DES_H
#define SIMPLE_DES_H
#include "simple_rand_compat.h"
#include <stdint.h>

void simple_des_ecb_encrypt(const uint8_t *input, uint8_t *output, const uint8_t *key, int enc);

typedef struct {
    uint8_t key[8];
} SIMPLE_DES_KEY;

// Replacement for OpenSSL DES_cblock and DES_key_schedule
typedef uint8_t simple_des_cblock[8];

static inline void simple_des_set_key(const simple_des_cblock rawkey, SIMPLE_DES_KEY *ks) {
    for (int i = 0; i < 8; i++) {
        ks->key[i] = rawkey[i];
    }
}

static inline void simple_des_random_key(simple_des_cblock *block) {
    RAND_bytes(*block, 8);
}

#endif // SIMPLE_DES_H
