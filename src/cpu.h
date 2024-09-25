#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include "utils.h"
#include "display.h"
#include "input.h"

// constants
#define MEMSIZE 4096 
#define REGCOUNT 16
#define STACKSIZE 16
#define FONTSET_SIZE 80
#define SPRITE_WIDTH 8

// cpu state:
struct chip8_cpu {
	uint16_t current_op_code;
	uint8_t memory[MEMSIZE];
	// memory representation:
	// 0x000 - 0x1FF font data
	//-->	0x050 - 0x0A0 - built in font data
	// 0x200 - 0xFFF - game data
	uint8_t registers[REGCOUNT];
	uint16_t program_counter;			//needs to be 12 bits wide
	uint16_t I_address_register;		//needs to be 12 bits wide	
	uint8_t delay_timer;
	uint8_t sound_timer;				//buzzes when reaching 0

	uint16_t stack[STACKSIZE];
	uint16_t s_pointer;

	// keypress keypad abstraction
	uint8_t keypad[KEYBOARD_SIZE];

	// any new graphics to render?
	uint8_t drawFlag;
	display display;
};

typedef struct chip8_cpu chip8_cpu;

void initialize(chip8_cpu* cpu);
void load_game(const char* gamepath); void emulate_cycle(chip8_cpu* cpu);
void emulate_cycle(chip8_cpu* cpu);
void handle_op(chip8_cpu* cpu);

// Instructions
// Store in register
void OP_6XNN(chip8_cpu* cpu);
void OP_8XY0(chip8_cpu* cpu);
// Addition
void OP_7XNN(chip8_cpu* cpu);
void OP_8XY4(chip8_cpu* cpu);
// Subtraction
void OP_8XY5(chip8_cpu* cpu);
void OP_8XY7(chip8_cpu* cpu);
// Bitwise
void OP_8XY2(chip8_cpu* cpu);
void OP_8XY1(chip8_cpu* cpu);
void OP_8XY3(chip8_cpu* cpu);
// Shift
void OP_8XY6(chip8_cpu* cpu);
void OP_8XYE(chip8_cpu* cpu);
void OP_CXNN(chip8_cpu* cpu);
// Flow control
void OP_1NNN(chip8_cpu* cpu);
void OP_BNNN(chip8_cpu* cpu);
// Subroutines
void OP_2NNN(chip8_cpu* cpu);
void OP_00EE(chip8_cpu* cpu);

