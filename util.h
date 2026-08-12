#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdint.h>

#define PRINT_BITS(T, N)                       \
        for (int i = (N - 1); i >= 0; i--) {   \
            printf("%d", (T >> i) & 1);        \
        } printf("\n")


uint8_t check_bit(uint16_t i, uint16_t nbit) {
    return (i >> nbit) & 1;
}

uint16_t reset_bit(uint16_t i, uint16_t nbit) {
    uint16_t x = ~(1 << nbit);
    return (i & x);
}

uint16_t set_bit(uint16_t i, uint16_t nbit) {
    uint16_t x = (1 << nbit);
    return (i | x);
}

uint16_t sign_extend(uint16_t n, uint16_t sign_bit) {
    if (((n >> sign_bit) & 1) == 1) {
        uint16_t extended = 0xffff << (sign_bit + 1);
        extended |= n;
        return extended;
    }
    return n;
}

#endif // UTIL_H