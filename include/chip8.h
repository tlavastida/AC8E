/*
chip8.h
declaration of chip8 class for chip8 emulator

https://en.wikipedia.org/wiki/CHIP-8
*/

#ifndef CHIP8_H
#define CHIP8_H

class chip8 {

private:
	static const int MEMSIZE = 4096;	
	static const int REGCOUNT = 16;	
	//memory map of system:	
	//0x000 - 0x1FF - chip8 interpreter
	//0x050 - 0x0A0 - for built in 4x5 pixel font set (0-F)
	//ox200 - 0xFFF - program ROM and work RAM

	unsigned short opc;				//opcode of next instruction
	unsigned char mem[MEMSIZE];		//memory bank
	unsigned char v_reg[REGCOUNT]; 	//register file, 15 registers, and 16th holds flags

	unsigned short i_reg; 			//Index register, actually 12 bits not 16
	unsigned short pc_reg;			//program counter, actually 12 bits not 16

	unsigned char gfx[64 * 32]; 	//pixels are black or white (1 or 0)

	unsigned char delay_timer;		//timer counts down to 0 when > 0
	unsigned char sound_timer;		//timer counts down to 0 when > 0

	unsigned short stack[16];		//stack - stores return addresses
	unsigned short sp;				//stack pointer - index of next address to return to

	unsigned char key[16];			//current state of key

	bool draw_flag; 				//set to true when something new has been drawn

public:

	chip8();
	~chip8();

	void initialize();
	void emulate_cycle();
	bool load_program(const char* filename);

};

#endif