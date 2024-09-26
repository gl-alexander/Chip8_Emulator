#include "cpu.h"
#include "stdio.h"
#include "fcntl.h"
#include "unistd.h"

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

void load_game(chip8_cpu* cpu, const char* gamepath) {
    char buffer[0x1000];
    int fd = open(gamepath, O_RDONLY);
    if (fd < 0) {
        printf("Could not find file %s\n", gamepath);
        exit(1);
    }
    int n = read(fd, buffer, sizeof(buffer));
    for (int i = 0; i < n; i++) {
        cpu->memory[START_ADDRESS + i] = buffer[i];
    }
}

void emulate_cycle(chip8_cpu* cpu) {
    // Fetch operation
    u16 pc = cpu->program_counter;
    cpu->current_op_code = ((cpu->memory[pc] << 8) | cpu->memory[pc + 1]);
    cpu->program_counter += 2;
    handle_op(cpu);
}

void handle_op(chip8_cpu* cpu) {
    u16 opcode = cpu->current_op_code;
    u16 b1, b2, b3, b4;
    b1 = (opcode & 0xf000) >> 12u;
    b2 = (opcode & 0x0f00); 
    b3 = (opcode & 0x00f0); 
    b4 = (opcode & 0x000f); 

    switch (b1) {
        case 0x1: OP_1NNN(cpu); break;
        case 0x2: OP_2NNN(cpu); break;
        case 0x3: OP_3XNN(cpu); break;
        case 0x4: OP_4XNN(cpu); break;
        case 0x5: OP_5XY0(cpu); break;
        case 0x6: OP_6XNN(cpu); break;
        case 0x7: OP_7XNN(cpu); break;
        case 0x9: OP_9XY0(cpu); break;
        case 0xA: OP_ANNN(cpu); break;
        case 0xB: OP_BNNN(cpu); break;
        case 0xC: OP_CXNN(cpu); break;
        case 0xD: OP_DXYN(cpu); break;
        case 0x8: {
            switch (b4) {
                case 0x0: OP_8XY0(cpu); break;
                case 0x1: OP_8XY1(cpu); break;
                case 0x2: OP_8XY2(cpu); break;
                case 0x3: OP_8XY3(cpu); break;
                case 0x4: OP_8XY4(cpu); break;
                case 0x5: OP_8XY5(cpu); break;
                case 0x6: OP_8XY6(cpu); break;
                case 0x7: OP_8XY7(cpu); break;
                case 0xE: OP_8XYE(cpu); break;
            }
            break;
        }
        case 0x0: {
            if (b3 != 0 || b2 != 0x00e0) break;
            switch (b4) {
                case 0x0: OP_00E0(cpu); break;
                case 0xE: OP_00EE(cpu); break;
            }
            break;
        }
        case 0xE: {
            switch (b3 + b4) {
                case 0xA1: OP_EXA1(cpu); break;
                case 0x9E: OP_EX9E(cpu); break;
            }
            break;
        }
        case 0xF: {
            switch (b3 + b4) {
                case 0x07: OP_FX07(cpu); break;
                case 0x0A: OP_FX0A(cpu); break;
                case 0x15: OP_FX15(cpu); break;
                case 0x18: OP_FX18(cpu); break;
                case 0x1E: OP_FX1E(cpu); break;
                case 0x29: OP_FX29(cpu); break;
                case 0x33: OP_FX33(cpu); break;
                case 0x55: OP_FX55(cpu); break;
                case 0x65: OP_FX65(cpu); break;
            }
        }

    }
}

u16 getVX(u16 opcode) {
    return ((opcode & 0x0f00) >> 8);
}
u16 getVY(u16 opcode) {
    return ((opcode & 0x00f0) >> 4);
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
    cpu->stack[cpu->s_pointer] = cpu->program_counter + 2;
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
void OP_3XNN(chip8_cpu* cpu) {
    u16 VX = getVX(cpu->current_op_code);
    u8 num = cpu->current_op_code & 0x00ff;
    if (cpu->registers[VX] == num) {
        cpu->program_counter += 2; // skip following instruction
    }
}
void OP_5XY0(chip8_cpu* cpu) {
    u16 VX = getVX(cpu->current_op_code);
    u16 VY = getVY(cpu->current_op_code);
    if (cpu->registers[VX] == cpu->registers[VY]) {
        cpu->program_counter += 2; 
    }
}
void OP_4XNN(chip8_cpu* cpu) {
    u16 VX = getVX(cpu->current_op_code);
    u8 num = cpu->current_op_code & 0x00ff;
    if (cpu->registers[VX] != num) {
        cpu->program_counter += 2; 
    }
}
void OP_9XY0(chip8_cpu* cpu) {
    u16 VX = getVX(cpu->current_op_code);
    u16 VY = getVY(cpu->current_op_code);
    if (cpu->registers[VX] != cpu->registers[VY]) {
        cpu->program_counter += 2; 
    }
}
void OP_FX15(chip8_cpu* cpu) {
    u16 VX = getVX(cpu->current_op_code);
    cpu->delay_timer = cpu->registers[VX];
}
void OP_FX07(chip8_cpu* cpu) {
    u16 VX = getVX(cpu->current_op_code);
    cpu->registers[VX] = cpu->delay_timer;
}
void OP_FX18(chip8_cpu* cpu) {
    u16 VX = getVX(cpu->current_op_code);
    cpu->sound_timer = cpu->registers[VX];
}
void OP_FX0A(chip8_cpu* cpu) {
    u16 VX = getVX(cpu->current_op_code);
    bool found = false;
    for (u8 i = 0; i < KEYBOARD_SIZE; i++) {
        if (cpu->keypad[i]) {
            cpu->registers[VX] = i;
            found = true;
            break;
        }
    }
    if (!found) {
        cpu->program_counter -= 2; // effectively executes a while loop
    }
} 
void OP_EX9E(chip8_cpu* cpu) {
    u16 VX = getVX(cpu->current_op_code);
    u8 value = cpu->registers[VX];
    if (value > KEYBOARD_SIZE) {
        printf("No such key...\n");
        exit(1);
    }
    if (cpu->keypad[value]) {
        cpu->program_counter += 2;
    }
}
void OP_EXA1(chip8_cpu* cpu) {
    u16 VX = getVX(cpu->current_op_code);
    u8 value = cpu->registers[VX];
    if (value > KEYBOARD_SIZE) {
        printf("No such key...\n");
        exit(1);
    }
    if (cpu->keypad[value] == 0) {
        cpu->program_counter += 2;
    }
}
void OP_ANNN(chip8_cpu* cpu) {
    u8 num = cpu->current_op_code & 0x0fff;
    cpu->I_address_register = num;
}
void OP_FX1E(chip8_cpu* cpu) {
    u16 VX = getVX(cpu->current_op_code);
    cpu->I_address_register += cpu->registers[VX];
}

void drawSpriteRow(chip8_cpu* cpu, u8 x, u8 y, u8 data) {
    for (int i = 0; i < SPRITE_WIDTH; i++) {
        u8 pixel = data & (0x80 >> i);
        if (pixel == 1) {
            if (cpu->display.screen[y][x + i] == 1) {
                cpu->registers[CARRY_BIT] = 1;
            }
            // This draw function draws the width of the passed row of bits, row stays the same
            // Note that the drawing is done by XOR-ing the current screen
            cpu->display.screen[y][x + i] ^= pixel;
        }
    }
}

void OP_DXYN(chip8_cpu* cpu) {
    // Draw sprite at reg[X] reg[Y] on the screen with N bytes of sprite data
    // Starting at the address I in memory
    // Drawing from top to bottom
    u16 VX = getVX(cpu->current_op_code);
    u16 VY = getVY(cpu->current_op_code);
    u8 height = cpu->current_op_code & 0x000f;

    u16 x, y;
    x = cpu->registers[VX] % WIDTH;
    y = cpu->registers[VY] % HEIGHT;
    cpu->registers[CARRY_BIT] = 0;

    if (x >= WIDTH - SPRITE_WIDTH || y >= HEIGHT - height) {
        printf("Draw call outside of screen range. X = %d, Y = %d...\n", x, y);
        exit(1);
    }
    for (int i = 0; i < height; i++) {
        u8 data = cpu->memory[cpu->I_address_register + i];
        drawSpriteRow(cpu, x, y + i, data); // row changes for each byte of sprite data
    }
    cpu->drawFlag = 1;
}

void OP_00E0(chip8_cpu* cpu) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            cpu->display.screen[y][x] = 0;
        }
    }
    cpu->drawFlag = 1;
}
void OP_FX29(chip8_cpu* cpu) {
    u16 VX = getVX(cpu->current_op_code);
    cpu->I_address_register = FONTSET_START_ADDRESS + cpu->registers[VX] * 5;
}
void OP_FX33(chip8_cpu* cpu) {
    u16 VX = getVX(cpu->current_op_code);
    u8 value = cpu->registers[VX];
    cpu->memory[cpu->I_address_register + 2] = value % 10;
    value /= 10;
    cpu->memory[cpu->I_address_register + 1] = value % 10;
    value /= 10;
    cpu->memory[cpu->I_address_register] = value % 10;
}
void OP_FX55(chip8_cpu* cpu) {
    u16 VX = getVX(cpu->current_op_code);
    for (int i = 0; i < VX; i++) {
        cpu->memory[cpu->I_address_register + i] = cpu->registers[i];
    }
    cpu->I_address_register += VX + 1;
} 
void OP_FX65(chip8_cpu* cpu) {
    u16 VX = getVX(cpu->current_op_code);
    for (int i = 0; i < VX; i++) {
        cpu->registers[i] = cpu->memory[cpu->I_address_register + i];
    }
    cpu->I_address_register += VX + 1;
} 
