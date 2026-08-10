#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define MEMORY_SIZE 65536
#define REGISTER_COUNT 8

uint16_t memory[MEMORY_SIZE];
uint16_t R[REGISTER_COUNT];
uint16_t pc;

typedef enum {
    BR = 0,
    ADD = 0b0001,
    AND = 0b0101,
    JMP = 0b1100,
    JSR = 0b0100,
    LD = 0b0010,
    LDI = 0b1010,
    LDR = 0b0110,
    LEA = 0b1110,
    NOT = 0b1001,
    RET = 0b1100,
    ST = 0b0011,
    STI = 0b1011,
    STR = 0b0111,
    TRAP = 0b1111,
    RESERVED = 0b1101
} OPCODE;

static void decode(uint16_t instruction);

#endif // CPU_H
