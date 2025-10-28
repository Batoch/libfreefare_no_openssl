#include "simple_des.h"

#ifndef DES_ENCRYPT
#define DES_ENCRYPT 1
#endif
#ifndef DES_DECRYPT
#define DES_DECRYPT 0
#endif
#ifndef SIMPLE_DES_COMPAT_H
#define SIMPLE_DES_COMPAT_H

// Replacement for OpenSSL DES_cblock and DES_key_schedule
typedef simple_des_cblock DES_cblock;

typedef SIMPLE_DES_KEY DES_key_schedule;

// Replacement for OpenSSL DES_set_key
static inline int DES_set_key(const DES_cblock *key, DES_key_schedule *schedule) {
    simple_des_set_key((const uint8_t *)key, schedule);
    return 0;
}

// Replacement for OpenSSL DES_ecb_encrypt
static inline void DES_ecb_encrypt(const DES_cblock *input, DES_cblock *output, const DES_key_schedule *ks, int enc) {
    // Use the stored key instead of casting the structure - this fixes the error
    simple_des_ecb_encrypt((const uint8_t *)input, (uint8_t *)output, ks->key, enc);
}

static inline void DES_random_key(DES_cblock *block) {
    simple_des_random_key(block);
}

#endif // SIMPLE_DES_COMPAT_H
