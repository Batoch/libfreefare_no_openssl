#ifndef SIMPLE_RAND_COMPAT_H
#define SIMPLE_RAND_COMPAT_H
#include "simple_rand.h"

// OpenSSL RAND_bytes compatibility
static inline int RAND_bytes(unsigned char *buf, int num) {
    return simple_rand_bytes(buf, (size_t)num);
}

#endif // SIMPLE_RAND_COMPAT_H
