#include "simple_rand.h"

// Simple (not cryptographically secure) random bytes generator
int simple_rand_bytes(uint8_t *buf, size_t len) {
    static int seeded = 0;
    if (!seeded) {
//        srand(1);
        seeded = 1;
    }
    for (size_t i = 0; i < len; ++i) {
        buf[i] = rand() & 0xFF;
    }
    return 1; // Always succeeds
}
