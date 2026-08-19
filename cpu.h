#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define MEMORY_SIZE 65536
#define REGISTER_COUNT 8
#define KEYBOARD_STATUS 0xFE00
#define KEYBOARD_DATA 0xFE02

uint16_t memory[MEMORY_SIZE];
uint16_t R[REGISTER_COUNT];
uint16_t flags;
uint16_t pc;

typedef enum {
    BR = 0,
    ADD = 0b0001,
    AND = 0b0101,
    JMP = 0b1100,
    JSR = 0b0100,
    LD = 0 b0010,
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

enum {
    TRAP_GETC = 0x20,
    TRAP_OUT = 0x21,
    TRAP_PUTS = 0x22,
    TRAP_INT = 0x23,
    TRAP_PUTB = 0x24,
    TRAP_HALT = 0x25
};

enum Flag {
    FL_P = 1,
    FL_Z = 1 << 1,
    FL_N = 1 << 2
};

bool running;
void run();

#endif // CPU_H
