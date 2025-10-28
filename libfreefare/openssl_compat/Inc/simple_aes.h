#ifndef SIMPLE_AES_H
#define SIMPLE_AES_H
#include <stdint.h>
#include <string.h>

#define AES_BLOCK_SIZE 16
#define AES_MAXNR 14
typedef struct {
	uint8_t rd_key[4 * (AES_MAXNR + 1)];
	int rounds;
} SIMPLE_AES_KEY;

int simple_aes_set_encrypt_key(const uint8_t *userKey, int bits, SIMPLE_AES_KEY *key);
int simple_aes_set_decrypt_key(const uint8_t *userKey, int bits, SIMPLE_AES_KEY *key);
void simple_aes_encrypt(const uint8_t *in, uint8_t *out, const SIMPLE_AES_KEY *key);
void simple_aes_decrypt(const uint8_t *in, uint8_t *out, const SIMPLE_AES_KEY *key);

#endif // SIMPLE_AES_H
