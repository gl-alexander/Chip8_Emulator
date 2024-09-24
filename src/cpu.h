#include <stdint.h>
#include "utils.h"
#include "display.h"
#include "input.h"

// constants
#define MEMSIZE 4096 
#define REGCOUNT 16
#define STACKSIZE 16

// cpu state:
struct chip8_cpu {
	uint16_t current_op_code;
	uint8_t memory[MEMSIZE];
	// memory representation:
	// 0x000 - 0x1FF font data
	//-->	0x050 - 0x0A0 - built in font data
	// 0x200 - 0xFFF - game data
	uint8_t V_registers[REGCOUNT];
	uint16_t program_counter;			//needs to be 12 bits wide
	uint16_t I_address_register;		//needs to be 12 bits wide	
	uint8_t delay_timer;
	uint8_t sound_timer;				//buzzes when reaching 0
										//
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
void store_keys(chip8_cpu* cpu);



