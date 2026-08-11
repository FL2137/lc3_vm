#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdint.h>


#define PRINT_BITS(T, N)                \
        for (int i = 0; i < N; i++) {   \
            printf("%d", (T >> i) & 1);  \
        } printf("\n")

#endif // UTIL_H