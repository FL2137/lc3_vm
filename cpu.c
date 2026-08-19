#include "cpu.h"
#include "util.h"

#include <errno.h>

static void init() {
    flags = FL_Z;
    pc = 0x3000;
    running = true;
}

static void load_program(char* path) {
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        printf("fopen() failure: %d\n", errno);
        exit(EXIT_FAILURE);
    }


    fclose(file);
}

void run() {
    init();

    while (running) {
        // fetch
        uint16_t instruction = memory_read(pc);
        pc++;
    
        execute(instruction);
    }

}

static uint16_t memory_read(uint16_t addr) {
    if (addr == KEYBOARD_STATUS) {

    }
    
    return memory[addr];
}

static uint16_t memory_write(uint16_t addr, uint16_t data) {

    memory[addr] = data;
}

static void update_flags(uint16_t R) {
    if (R == 0) {
        flags = FL_Z;
    }
    else if ((R >> 15) == 1) {
        flags = FL_N;
    }
    else {
        flags = FL_P;
    }
}

static void br(uint16_t instruction) {
    uint16_t pc_offset = sign_extend(instruction & 0x1ff, 9);

    uint16_t condition = (instruction >> 9) & 0b111;

    if (condition & flags) {
        pc += pc_offset;
    }
}

static void add(uint16_t instruction) {
    
    uint16_t dst = (instruction >> 9) & 0b111;
    uint16_t src1 = (instruction >> 6) & 0b111;
    uint16_t src2;

    uint8_t is_immediate = (instruction >> 4) & 1;
    if (is_immediate == 1) { // this doesn't work as of now cuz twos complement is dumb
        if (((instruction >> 4) & 1) == 1) {
            uint16_t imm5 = instruction & 0b11111;
            imm5 = sign_extend(imm5, 5);
            R[dst] = R[src1] + imm5;
        }
    }
    else {
        src2 = instruction & 0b111;
        R[dst] = R[src1] + R[src2];
    }

    update_flags(R[dst]);
}

static void not(uint16_t instruction) {
    uint16_t dst = (instruction >> 9) & 0b111;
    uint16_t src1 = (instruction >> 6) & 0b111;

    R[dst] = ~R[src1];
    update_flags(R[dst]);
}

static void and(uint16_t instruction) {
    uint16_t dst = (instruction >> 9) & 0b111;
    uint16_t src1 = (instruction >> 6) & 0b111;
    uint16_t src2;

    uint8_t is_immediate = (instruction >> 4) & 1;
    if (is_immediate == 1) { 
        if (((instruction >> 4) & 1) == 1) {
            uint16_t imm5 = instruction & 0b11111;
            imm5 = sign_extend(imm5, 5);
            R[dst] = R[src1] & imm5;
        }
    }
    else {
        src2 = instruction & 0b111;
        R[dst] = R[src1] & R[src2];
    }

    update_flags(R[dst]);
}

static void jmp(uint16_t instruction) {
    uint16_t base_reg = (instruction >> 6) & 0b111;
    pc = R[base_reg];
}

static void jsr(uint16_t instruction) {
    uint8_t is_jsrr = (instruction >> 11) & 1;
    if (is_jsrr == 1) {
        uint16_t pc_offset = sign_extend(instruction & 0x7ff, 11);
        pc += pc_offset;
    }
    else {
        R[7] = pc;
        uint16_t base_reg = (instruction >> 6) & 0b111;
        pc = R[base_reg];
    }
}

static void ld(uint16_t instruction) {
    uint16_t pc_offset = sign_extend(instruction & 0x1ff, 9);
    uint16_t reg = (instruction >> 9) & 0b111;
    R[reg] = memory_read(pc + pc_offset);
    update_flags(R[reg]);
}

static void ldi(uint16_t instruction) {
    uint16_t reg = (instruction >> 9) & 0b111;
    uint16_t pc_offset = sign_extend(instruction & 0x1ff, 9);
    R[reg] = pc + pc_offset;
    update_flags(R[reg]);
}

static void ldr(uint16_t instruction) {
    uint16_t reg1 = (instruction >> 9) & 0b111;
    uint16_t reg2 = (instruction >> 6) & 0b111;
    uint16_t offset = sign_extend(instruction & 0b111111, 6);
    R[reg1] = memory_read(R[reg2] + offset);
    update_flags(R[reg1]);
}

static void st(uint16_t instruction) {
    uint16_t pc_offset = sign_extend(instruction & 0x1ff, 9);
    uint16_t reg = (instruction >> 9) & 0b111;
    memory_write(pc + pc_offset, R[reg]);
}

static void sti(uint16_t instruction) {
    uint16_t pc_offset = sign_extend(instruction & 0x1ff, 9);
    uint16_t reg = (instruction >> 9) & 0b111;
    memory_write(memory_read(pc + pc_offset), R[reg]);
}

static void str(uint16_t instruction) {
    uint16_t reg1 = (instruction >> 9) & 0b111;
    uint16_t reg2 = (instruction >> 6) & 0b111;
    uint16_t offset = sign_extend(instruction & 0x3f, 6);
    memory_write(R[reg2] + offset, R[reg1]);

}

static void trap(uint16_t code) {
    switch (code) {
        case TRAP_GETC: {
            R[0] = (uint16_t)getchar();
            update_flags(R[0]);
            break;
        }
        case TRAP_OUT: {
            putc((char)R[0], stdout);
            fflush(stdout);
            break;
        }
        case TRAP_PUTS: {
            uint16_t* c = memory + R[0];
            while (*c) {
                putc((char)*c, stdout);
                ++c;
            }
            fflush(stdout);
            break;
        }
        case TRAP_INT: {
            printf("Input a char: ");
            char c = getchar();
            putc(c, stdout);
            fflush(stdout);
            R[0] = (uint16_t)c;
            update_flags(R[0]);
            break;
        }
        case TRAP_PUTB: {
            uint16_t* c = memory + R[0];
            while (*c) {
                char c1 = (*c) & 0xFF;
                putc(c, stdout);
                char c2 = (*c) >> 8;
                if (c2) {
                    putc(c2, stdout);
                }
                ++c;
            }
            fflush(stdout);
            break;
        }
        case TRAP_HALT: {
            printf("HALT\n");
            fflush(stdout);
            running = false;
            break;
        }
    }
}

void execute(uint16_t instruction) {
    uint8_t op = instruction >> 12;

    switch (op) {
        case BR: {
            br(instruction);
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
        case LD: {
            ld(instruction);
            break;
        }
        case LDI: {
            ldi(instruction);
            break;
        }
        case LDR: {
            ldr(instruction);
            break;
        }
        case TRAP: {
            R[7] = pc;
            trap(instruction & 0xff);
            break;
        }
    }
}
