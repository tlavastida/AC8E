/*
chip8.cpp
implementation of chip8 member functions
http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
https://en.wikipedia.org/wiki/CHIP-8
*/

#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>

unsigned char chip8_fontset[80] = {
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

chip8::chip8() {

}

chip8::~chip8() {

}

void chip8::initialize() {

	pc_reg = 0x200; //pc always starts at 0x200
	opc = 0;		//reset opcode
	i_reg = 0;		//reset index reg
	sp = 0;			//reset stack pointer

	//clear display
	for(int i = 0; i < 64*32; ++i) {
		gfx[i] = 0x0;
	}
	//clear stack
	//clear reg file (v_reg[0] to v_reg[15])
	for(int i = 0; i < 16; ++i) {
		stack[i] = 0;
		v_reg[i] = 0;
		key[i] = 0;
	}
	//clear mem
	for(int i = 0; i < 4096; ++i) {
		mem[i] = 0;
	}

	for(int i= 0; i < 80; ++i) {
		mem[i] = chip8_fontset[i];	//load font set
	}
	//reset timers
	delay_timer = 0;
	sound_timer = 0;

	draw_flag = true;

	//seed rng
	srand(time(NULL));
}

void chip8::emulate_cycle() {
	//fetch next instruction
	opc = mem[pc_reg] << 8 | mem[pc_reg + 1];  //shift 1st byte 8 bits to the left then OR w/ 2nd byte

	switch(opc & 0xF000) {
		//mask out lower 12 bits of instruction

		case 0x0000:
			switch(opc & 0x00FF) {
				case 0x00E0: //clears the screen
					for( int i = 0; i < 64*32; ++i) {
						gfx[i] = 0x0;
					}
					draw_flag = true;
					pc_reg += 2;
					break;

				case 0x00EE: //returns from a subroutine
					--sp;
					pc_reg = stack[sp] + 2;
					break;

				default: printf("Unknown opcode [0x000]: 0x%X\n", opc);
			}
			break;

		case 0x1000:	//0x1NNN: jumps to adress NNN
			pc_reg = opc & 0xFFF;
			break;

		case 0x2000:	//2NNN: calls subroutine at NNN
			stack[sp] = pc_reg;  	//store return address
			++sp;		//increment stack pointer
			pc_reg = opc & 0x0FFF;	//set PC to jump address
			break;

		case 0x3000:	//0x3XNN skips next instruction if v_reg[X] == NN
			if( v_reg[(opc & 0x0F00) >> 8] == (opc & 0x00FF) )
				pc_reg += 4; //skip next instruction
			else
				pc_reg += 2; //dont skip
			break;

		case 0x4000:	//0x4XNN: skips next instruction of v_reg[X]!=NN
			if( v_reg[(opc & 0x0F00) >> 8] != (opc & 0x00FF) )
				pc_reg += 4; //skip next instruction
			else
				pc_reg += 2; //dont skip
			break;

		case 0x5000:	//0x5XY0: skips next instruction if VX == VY
			if( v_reg[(opc & 0x0F00) >> 8] == v_reg[(opc & 0x00F0) >> 4] )
				pc_reg += 4; //skip
			else
				pc_reg += 2; //dont skip
			break;

		case 0x6000:	//0x6XNN: sets v_reg[X] to NN
			v_reg[(opc & 0x0F00) >> 8] = opc & 0x00FF;
			pc_reg += 2;
			break;

		case 0x7000:	//0x7XNN: adds NN to VX
			v_reg[(opc & 0x0F00) >> 8] += opc & 0x00FF;
			pc_reg += 2;
			break;

		case 0x8000:
			switch(opc & 0x00F) {

				case 0x0000:	//0x8XY0 : set vx to vy
					v_reg[(opc & 0x0F00) >> 8] = v_reg[(opc & 0x00F0) >> 4];
					pc_reg += 2;
					break;

				case 0x0001: //0x8XY1: VX = VX | VY
					v_reg[(opc & 0x0F00) >> 8] |= v_reg[(opc & 0x00F0) >> 4];
					pc_reg += 2;
					break;

				case 0x0002: //0x8XY2: vx = vx & vy
					v_reg[(opc & 0x0F00) >> 8] &= v_reg[(opc & 0x00F0) >> 4];
					pc_reg += 2;
					break;

				case 0x0003:  //0x8XY3: vx = vx ^ vy (XOR)
					v_reg[(opc & 0x0F00) >> 8] ^= v_reg[(opc & 0x00F0) >> 4];
					pc_reg += 2;
					break;

				case 0x0004: //0x8XY4: add VY to VX, set VF to 1 if a carry occurred and 0 if not
					if(v_reg[(opc & 0x00F0) >> 4] > (0xFF - v_reg[(opc & 0x0F00) >> 8]))
						v_reg[0xF] = 1;
					else 
						v_reg[0xF] = 0;
					v_reg[(opc & 0x0F00) >> 8] += v_reg[(opc & 0x00F0) >> 4];  //do the addition
					pc_reg += 2;	
					break;

				case 0x0005: //0x8XY5: vx = vx - vy; set vf to 0 when there is a borrow, 1 otherwise
					if(v_reg[(opc & 0x00F0) >> 4] > v_reg[(opc & 0x0F00) >> 8])
						v_reg[0xF] = 0;
					else
						v_reg[0xF] = 1;
					v_reg[(opc & 0x0F00) >> 8] -= v_reg[(opc & 0x00F0) >> 4];
					pc_reg += 2;
					break;

				case 0x0006: //0x8XY6: shift vx right by one, vf is set to value of lsb of vx before shift
					v_reg[0xF] = v_reg[(opc & 0x0F00) >> 8] & 0x0001;
					v_reg[(opc & 0x0F00) >> 8] >>= 1;
					pc_reg += 2;
					break;
				case 0x0007: // vx = vy - vx; vf = 0 on borrow, 1 otherwise
					if(v_reg[(opc & 0x00F0) >> 4] < v_reg[(opc & 0x00F0) >> 4])
						v_reg[0xF] = 0;
					else
						v_reg[0xF] = 1;
					v_reg[(opc & 0x0F00) >> 8] = v_reg[(opc & 0x00F0) >> 4] - v_reg[(opc & 0x00F0) >> 4];
					pc_reg += 2;
					break;

				case 0x000E: //shift vx left by one, vf set to value of msb of vx before shift
					v_reg[0xF] = v_reg[(opc & 0x0F00) >> 8] >> 7;
					v_reg[(opc & 0x0F00) >> 8] >>= 1;
					pc_reg += 2;
					break;

				default:
					printf("unknown opcode: 0x%X\n", opc);
			}
			break;

		case 0x9000: //0x9XY0: skips next instr if vx != vy
			if( v_reg[(opc & 0x0F00) >> 8] != v_reg[(opc & 0x00F0) >> 4] )
				pc_reg += 4;
			else
				pc_reg += 2;
			break;

		case 0xA000: 	//ANNN: sets i_reg to address given by NNN
			i_reg = opc & 0x0FFF; 	//mask out MSB
			pc_reg += 2;
			break;

		case 0xB000:	//BNNN: jumps to the address NNN plus v_reg[0]
			pc_reg = (opc & 0x0FFF) + v_reg[0];
			break;

		case 0xC000: //0xCXNN: sets vx to a random number masked by NN
			v_reg[(opc & 0x0F00) >> 8] = (rand() % 0xFF) & (opc & 0xFF);
			pc_reg += 2;
			break;

		case 0xD000: //0xDXYN 
		{	
			unsigned short x = v_reg[(opc & 0x0F00) >> 8];
			unsigned short y = v_reg[(opc & 0x00F0) >> 4];
			unsigned short height = opc & 0x000F;
			unsigned short px;

			v_reg[0xF] = 0; // set to zero initially, then detect if a pixel changed from 1 -> 0
			for( int yline = 0; yline < height; ++yline) {
				px = mem[i_reg + yline]; // get next pixel
				for(int xline = 0; xline < 8; ++ xline) {
					if( (px & (0x80 >> xline)) != 0 ) {
						if(gfx[(x + xline + 64*(y + yline))] == 1)
							v_reg[0xF] = 1;
						gfx[x + xline + 64*(y + yline)] ^= 1;
					}
				}
			}
			draw_flag = true;
			pc_reg += 2;
		}	
		break;

		case 0xE000:
			switch (opc & 0x00FF) {
				case 0x009E:  //ski[s next instr if key stored in vx is pressed
					if(key[v_reg[(opc & 0x0F000) >> 8]] != 0) 
						pc_reg += 4;
					else
						pc_reg += 2;
					break;
					
				case 0x00A1: //skips next instruction of key stores in vx isnt pressed
					if(key[v_reg[(opc & 0x0F00) >> 8]] == 0)
						pc_reg += 4;
					else
						pc_reg += 2;
					break;

				default:
					printf("unknown opcode: 0x%X\n", opc);
			}
			break;

		case 0xF000:
			switch (opc & 0x00FF) {
				case 0x0007: //sets vx to value of delay timer
					v_reg[(opc & 0x0F00) >> 8] = delay_timer;
					pc_reg += 2;
					break;

				case 0x000A: //a keypress is awaited, then stored in vx
				{
					bool key_press = false;

					for(int i = 0; i < 16; ++i) {
						if(key[i] != 0) {
							key_press = true;
							v_reg[(opc & 0x0F00) >> 8] = i;
						}
					}
					if(!key_press)
						return;
					pc_reg += 2;
				}
				break;

				case 0x0015: //set delay timer to vx
					delay_timer = v_reg[(opc & 0x0F00) >> 8];
					pc_reg += 2;
					break;

				case 0x0018: //set sound timer to vx
					sound_timer = v_reg[(opc & 0x0F00) >> 8];
					pc_reg += 2;
					break;

				case 0x001E: //adds vx to i
					if(i_reg + v_reg[(opc & 0x0F00) >> 8] > 0xFFF) 
						v_reg[0xF] = 1;
					else
						v_reg[0xF] = 0;
					i_reg += v_reg[(opc & 0x0F00) >> 8];
					pc_reg += 2;
					break;

				case 0x0029: //0xFX29 - sets I to location of sprite for char in vx
					i_reg = v_reg[(opc & 0x0F00) >> 8] * 0x5;
					pc_reg += 2;
					break;

				case 0x0033: //0xFX33-stores bcd representation of vx at addr i, i+1, and i+2
					mem[i_reg] = v_reg[(opc & 0x0F00) >> 8] / 100;
					mem[i_reg + 1] = (v_reg[(opc & 0x0F00) >> 8] / 10) % 10;
					mem[i_reg + 2] = (v_reg[(opc & 0x0F00) >> 8] % 100) % 10;
					pc_reg += 2;
					break;

				case 0x0055: //0xFX55-stores V0 through VX in memory starting at address i_reg
					for(int i = 0; i <= ((opc & 0x0F00) >> 8); ++i) {
						mem[i_reg + i] = v_reg[i];
					}
					i_reg += ((opc & 0x0F00) >> 8) + 1; //on original interpeter, i = i + X + 1
					pc_reg += 2;
					break;

				case 0x0065:
					for(int i = 0; i <= ((opc & 0x0F00) >> 8); ++i) {
						v_reg[i] = mem[i_reg + i];
					}
					i_reg += ((opc & 0x0F00) >> 8) + 1; //on original interpeter, i = i + X + 1
					pc_reg += 2;
					break;

				default:
					printf("unknown opcode: 0x%X\n", opc);
			}
			break;

		//default - error
		default: printf("Unknown opcode: 0x%X\n", opc);
	}

	if(delay_timer > 0) --delay_timer;

	if(sound_timer > 0) {
		if(sound_timer == 1)
			printf("BEEP!\n");
		--sound_timer;
	}
}

bool chip8::load_program(const char* filename) {
	initialize();
	printf("Loading: %s\n", filename);

	std::ifstream fin(filename, std::ios::binary | std::ios::ate);
	if(!fin.is_open()) {
		fputs("file error\n", stderr);
		return false;
	}

	//compute file size
	int file_size = fin.tellg();
	fin.seekg(0);

	char* buffer = (char*)malloc(file_size);
	if(buffer == NULL) {
		fputs("malloc failed\n", stderr);
		return false;
	}

	fin.read(buffer, file_size);

	//copy buffer into chip8 memory
	if( (4096 - 512) > file_size) {
		for (int i = 0; i < file_size; ++i) {
			mem[i+512] = (unsigned char)buffer[i];
		}
	}
	else {
		printf("Error: ROM too big for chip8 memory\n");
	}


	fin.close();
	free(buffer);
	return true;
}
