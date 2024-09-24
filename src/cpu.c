#include "cpu.h"

const u32 START_ADDRESS = 0x200;
const u32 FONTSET_START_ADDRESS = 0x50;

const u32 CARRY_BIT = 15;

// Sprite representation of the fontset
static uint8_t fontset[FONTSET_SIZE] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};


void initialize(chip8_cpu* cpu) {
    // ROM is loaded into address 0x200
    cpu->program_counter = START_ADDRESS;
    cpu->current_op_code = 0;
    cpu->I_address_register = 0;
    cpu->s_pointer = 0;

    // Load fonts
    for (int i = 0; i < FONTSET_SIZE; i++) {
        cpu->memory[FONTSET_START_ADDRESS + i] = fontset[i];
    }

    // seed random number 
    srand(time(NULL));
}

void emulate_cycle(chip8_cpu* cpu) {
    // Fetch operation
    cpu->current_op_code = 
    ((cpu->memory[cpu->program_counter] << 8) | cpu->memory[cpu->program_counter + 1]);

}

void handle_op(chip8_cpu* cpu) {
    u16 opcode = cpu->current_op_code;
    if (opcode > 0x1000) {
        OP_1NNN(cpu);
    }
}

u16 getVX(u16 opcode) {
    return ((opcode & 0x0f00) >> 16);
}
u16 getVY(u16 opcode) {
    return ((opcode & 0x00f0) >> 8);
}

void OP_6XNN(chip8_cpu* cpu) {
    u16 VX = getVX(cpu->current_op_code);
    u8 num = cpu->current_op_code & 0x00ff;
    cpu->registers[VX] = num;
}
void OP_8XY0(chip8_cpu* cpu) {
    u16 VX = getVX(cpu->current_op_code);
    u16 VY = getVY(cpu->current_op_code);
    cpu->registers[VX] = cpu->registers[VY];

}
void OP_7XNN(chip8_cpu* cpu) {
    u16 VX = getVX(cpu->current_op_code);
    u8 num = cpu->current_op_code & 0x00ff;
    cpu->registers[VX] = (cpu->registers[VX] + num) % 255;
}
void OP_8XY4(chip8_cpu* cpu) {
    u16 VX = getVX(cpu->current_op_code);
    u16 VY = getVY(cpu->current_op_code);
    u16 result = (u16)cpu->registers[VX] + (u16)cpu->registers[VY];
    if (result > 0xFF) {
        cpu->registers[CARRY_BIT] = 1;
    }
    else {
        cpu->registers[CARRY_BIT] = 0;
    }
    cpu->registers[VX] = (result % 0xff);
}
void OP_8XY5(chip8_cpu* cpu) {
    u16 VX = getVX(cpu->current_op_code);
    u16 VY = getVY(cpu->current_op_code);
    if (cpu->registers[VX] < cpu->registers[VY]) { // borrow
        cpu->registers[CARRY_BIT] = 0;
    }
    else {
        cpu->registers[CARRY_BIT] = 1;
    }
    cpu->registers[VX] -= cpu->registers[VY];
}
void OP_8XY7(chip8_cpu* cpu) {
    u16 VX = getVX(cpu->current_op_code);
    u16 VY = getVY(cpu->current_op_code);
    if (cpu->registers[VX] > cpu->registers[VY]) { 
        cpu->registers[CARRY_BIT] = 0;
    }
    else {
        cpu->registers[CARRY_BIT] = 1;
    }
    cpu->registers[VX] = cpu->registers[VY] - cpu->registers[VX];
}
void OP_8XY2(chip8_cpu* cpu) {
    u16 VX = getVX(cpu->current_op_code);
    u16 VY = getVY(cpu->current_op_code);

    cpu->registers[VX] &= cpu->registers[VY];
}
void OP_8XY1(chip8_cpu* cpu) {
    u16 VX = getVX(cpu->current_op_code);
    u16 VY = getVY(cpu->current_op_code);

    cpu->registers[VX] |= cpu->registers[VY];
}
void OP_8XY3(chip8_cpu* cpu) {
    u16 VX = getVX(cpu->current_op_code);
    u16 VY = getVY(cpu->current_op_code);

    cpu->registers[VX] ^= cpu->registers[VY];
}
void OP_8XY6(chip8_cpu* cpu) {
    u16 VX = getVX(cpu->current_op_code);
    u16 VY = getVY(cpu->current_op_code);

    cpu->registers[CARRY_BIT] = (cpu->registers[VY] & 1);
    cpu->registers[VX] = (cpu->registers[VY] >> 1);
}
void OP_8XYE(chip8_cpu* cpu) {
    u16 VX = getVX(cpu->current_op_code);
    u16 VY = getVY(cpu->current_op_code);

    cpu->registers[CARRY_BIT] = ((cpu->registers[VY] & 128) >> 7);
    cpu->registers[VX] = (cpu->registers[VY] << 1);
}
void OP_CXNN(chip8_cpu* cpu) {
    u16 VX = getVX(cpu->current_op_code);
    u8 mask = cpu->current_op_code & 0x00ff;
    cpu->registers[VX] = ((rand() % 0xFF) & mask);
}
void OP_1NNN(chip8_cpu* cpu) {
    cpu->program_counter =( cpu->current_op_code & 0xFFF);
}
void OP_BNNN(chip8_cpu* cpu) {
    u16 result = ((cpu->current_op_code & 0xFFF) + cpu->registers[0]);
    if (result > 0xFFF) {
        // bad address
        printf("Bad address: %d...\n", result);
        exit(1);
    }
    cpu->program_counter = result;
}
void OP_2NNN(chip8_cpu* cpu) {
    if (cpu->s_pointer >= 16) {
        printf("Stack overflow...\n");
        exit(1);
    }
    // Remember address of next instruction: 
    cpu->stack[cpu->s_pointer] = cpu->program_counter + 1;
    cpu->s_pointer++;
    // Set subroutine address start as current operation
    cpu->program_counter = ( cpu->current_op_code & 0xFFF); 
}
void OP_00EE(chip8_cpu* cpu) {
    if (cpu->s_pointer == 0) {
        printf("No subroutine executing currently...\n");
        exit(1);
    }
    // Fetch old address:
    cpu->s_pointer--;
    cpu->program_counter = cpu->stack[cpu->s_pointer];
}
