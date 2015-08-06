/*
	main.cpp
	main routine for chip8 emu project

	sketch for main routine
		create chip8 object
		setup graphics and input
		initialize chip8
		load a game
		start main loop
			emulate one cycle
			if draw_flag then draw_graphics()
			set state of chip8's keys
		end main loop
		cleanup
		close
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "chip8.h"
#include "sdl_funcs.h"
#include <fstream>


int process_args(const int& argc, char* argv[], std::string& str);



int main(int argc, char* argv[]) {
	std::string window_title = "AC8E: ";
	if(process_args(argc,argv,window_title) < 0) {
		return 1;
	}

/*
	chip8 my_chip8;
	if(!my_chip8.load_program(filename)) {
		std::cout << "failed to load " << filename << ", exiting ...\n";
		return 2;
	}

*/
	sdl_chip8_window chip8_window(window_title);
	//screen setup, do stuff with it

	SDL_Delay(2000);
	chip8_window.draw_pixels();
	SDL_Delay(500);

	for(int i = 0; i < 30; ++i) {
		chip8_window.randomize_pixels();
		chip8_window.draw_pixels();
		SDL_Delay(250);
	}


	return 0;
}

int process_args(const int& argc, char* argv[], std::string& str) {

	if(argc != 2) {
		std::cout << "usage: " << argv[0] << " <filename>\n";
		return -1;
	}
	std::string temp(argv[1]);
	str += temp;

	return 0;
}