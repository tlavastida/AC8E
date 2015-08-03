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
#include <SDL2/SDL.h>
#include "chip8.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* argv[]) {

	if(argc != 2) {
		std::cout << "usage: " << argv[0] << " <filename>\n";
		return 1;
	}

	const char* filename = argv[1];

	chip8 my_chip8;
	if(!my_chip8.load_program(filename)) {
		std::cout << "failed to load " << filename << ", exiting ...\n";
		return 2;
	}

	SDL_Window* chip8_window = NULL;
	SDL_Surface* chip8_surface = NULL;
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) { //initialize sdl
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return 3;
	}

	char str[50] = "chip8: ";
	strcat(str,filename);

	chip8_window = SDL_CreateWindow(str,SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
									SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

	if(chip8_window == NULL) {
		printf("window could not be created, SDL_Error: %s\n", SDL_GetError());
		return 4;
	}

	chip8_surface = SDL_GetWindowSurface(chip8_window);
	SDL_FillRect(chip8_surface, NULL, SDL_MapRGB(chip8_surface->format, 0x00,0x00,0x00));
	SDL_UpdateWindowSurface(chip8_window);
	SDL_Delay(2000);

	//screen setup, do stuff with it

	//cleanup
	SDL_DestroyWindow(chip8_window);

	SDL_Quit();

	return 0;
}