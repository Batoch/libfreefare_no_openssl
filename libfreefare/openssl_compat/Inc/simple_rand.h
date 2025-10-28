#ifndef SIMPLE_RAND_H
#define SIMPLE_RAND_H
#include <stdint.h>
#include <stdlib.h>

// Fill buf with len random bytes. Returns 1 on success, 0 on failure.
int simple_rand_bytes(uint8_t *buf, size_t len);

#endif // SIMPLE_RAND_H
