#include "cpu.h"

static void add(uint16_t instruction) {
    
    uint16_t dst = (instruction >> 9) & 0b111;
    uint16_t src1 = (instruction >> 6) & 0b111;
    uint16_t src2;

    uint8_t is_immediate = (instruction >> 4) & 1;
    if (is_immediate == 1) { // this doesn't work as of now cuz twos complement is dumb
        if (((instruction >> 4) & 1) == 1) {
            int x = 0b11110;
            x--;
            x = ~x;
        }
    }
    else {
        src2 = instruction & 0b111;
        R[dst] = R[src1] + R[src2];
    }
}

static void not(uint16_t instruction) {
    uint16_t dst = (instruction >> 9) & 0b111;
    uint16_t src1 = (instruction >> 6) & 0b111;

    R[dst] = ~R[src1];
}

static void and(uint16_t instruction) {
    uint16_t dst = (instruction >> 9) & 0b111;
    uint16_t src1 = (instruction >> 6) & 0b111;
    uint16_t src2;

    uint8_t is_immediate = (instruction >> 4) & 1;
    if (is_immediate == 1) { // this doesn't work as of now cuz twos complement is dumb
        if (((instruction >> 4) & 1) == 1) {
            int x = 0b11110;
            x--;
            x = ~x;
        }
    }
    else {
        src2 = instruction & 0b111;
        R[dst] = R[src1] & R[src2];
    }
}

static void jmp(uint16_t instruction) {
    uint16_t base_reg = (instruction >> 6) & 0b111;
    pc = R[base_reg];
}

static void jsr(uint16_t instruction) {
    uint8_t is_reg_the_base = (instruction >> 11) & 1;
    if (is_reg_the_base == 1) {
        R[7] = pc + 1;
        uint16_t base_reg = (instruction >> 6) & 0b111;
        pc = R[base_reg];
    }
    else {
        int x = 0b10000000100;
        int x2 = 0xFC04;
    }

}
 
void execute(uint16_t instruction) {
    uint8_t op = instruction >> 12;

    switch (op) {
        case BR: {

        } break;
        case ADD: {
            add(instruction);
            break;
        } 
        case AND: {
            and(instruction);
            break;
        } 
        case JMP: {
            jmp(instruction);
            break;
        }
        case JSR: {
            jsr(instruction);
            break;
        } 
    }
}
