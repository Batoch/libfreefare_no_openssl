#include "simple_aes.h"

// Replacement for OpenSSL AES_KEY
#define AES_BLOCK_SIZE 16
#ifndef SIMPLE_AES_COMPAT_H
#define SIMPLE_AES_COMPAT_H

// OpenSSL-compatible wrappers
#define AES_BLOCK_SIZE 16

typedef SIMPLE_AES_KEY AES_KEY; // For compatibility

static inline int AES_set_encrypt_key(const uint8_t *userKey, int bits, SIMPLE_AES_KEY *key) {
    return simple_aes_set_encrypt_key(userKey, bits, key);
}
static inline int AES_set_decrypt_key(const uint8_t *userKey, int bits, SIMPLE_AES_KEY *key) {
    return simple_aes_set_decrypt_key(userKey, bits, key);
}
static inline void AES_encrypt(const uint8_t *in, uint8_t *out, const SIMPLE_AES_KEY *key) {
    simple_aes_encrypt(in, out, key);
}
static inline void AES_decrypt(const uint8_t *in, uint8_t *out, const SIMPLE_AES_KEY *key) {
    simple_aes_decrypt(in, out, key);
}

#endif // SIMPLE_AES_COMPAT_H
