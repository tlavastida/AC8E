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

int process_args(const int& argc, char* argv[], char* str);


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 320;

int main(int argc, char* argv[]) {
	char window_title[50] = "\0";
	if(process_args(argc,argv,window_title) < 0) {
		return 1;
	}


	chip8 my_chip8;
	if(!my_chip8.load_program(filename)) {
		std::cout << "failed to load " << filename << ", exiting ...\n";
		return 2;
	}

	SDL_Window* chip8_window = NULL;		//application window
	SDL_Surface* chip8_surface = NULL;		//surface for application window

	if(!setup_sdl(chip8_window,chip8_surface,SCREEN_WIDTH,SCREEN_HEIGHT,window_title)) {
		printf("setup_sdl failed\n");
		return 3;
	}
	//screen setup, do stuff with it

	SDL_Delay(2000);


	//cleanup
	SDL_DestroyWindow(chip8_window);

	SDL_Quit();

	return 0;
}

int process_args(const int& argc, char* argv[], char* str) {

	if(argc != 2) {
		std::cout << "usage: " << argv[0] << " <filename>\n";
		return -1;
	}

	strcat(str,"AC8E: ");
	strcat(str,argv[1]);

	return 0;
}