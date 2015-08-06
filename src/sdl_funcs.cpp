/*
sdl_funcs.cpp
implementation of functions in sdl_funcs.h
*/

#include "sdl_funcs.h"
#include <algorithm> //for std::swap
#include <stdlib.h>
#include <time.h>
/*
sdl_chip8_window::sdl_chip8_window() {
	window = NULL;
	window_surface = NULL;
	if(!setup_sdl(window,window_surface,SCREEN_WIDTH,SCREEN_HEIGHT,"AC8E")) {
		printf("setup_sdl failed\n");
		exit(-1);
	}

	BLACK = SDL_MapRGB(window_surface->format, 0x00,0x00,0x00);
	WHITE = SDL_MapRGB(window_surface->format, 0xFF,0xFF,0xFF);

	setup_pixels();

	srand(time(NULL)); //seed rng
}
*/
sdl_chip8_window::sdl_chip8_window(std::string str = "AC8E") {
	window = NULL;
	window_surface = NULL;
	if(!setup_sdl(window,window_surface,SCREEN_WIDTH,SCREEN_HEIGHT,str.c_str())) {
		printf("setup_sdl failed\n");
		exit(-1);
	}

	BLACK = SDL_MapRGB(window_surface->format, 0x00,0x00,0x00);
	WHITE = SDL_MapRGB(window_surface->format, 0xFF,0xFF,0xFF);

	setup_pixels();

	srand(time(NULL)); //seed rng
}

sdl_chip8_window::~sdl_chip8_window() {
	SDL_DestroyWindow(window);
	SDL_Quit();
}

bool sdl_chip8_window::setup_sdl(SDL_Window*& w, SDL_Surface*& s, int scr_width, int scr_height, const char* str) {
	if( SDL_Init( SDL_INIT_VIDEO) < 0 ) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	w = SDL_CreateWindow(str, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
							scr_width, scr_height, SDL_WINDOW_SHOWN);

	if(w == NULL) {
		printf("Window could not be created, SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	s = SDL_GetWindowSurface(w);
	SDL_FillRect(s, NULL, SDL_MapRGB(s->format, 0x00,0x00,0x00));
	SDL_UpdateWindowSurface(w);

	return true;
}

void sdl_chip8_window::setup_pixels() {
	int x = 0;
	int y = 0;
	Uint32 even_color = BLACK;
	Uint32 odd_color = WHITE;
	for(int i = 0; i < 64*32; ++i) {
		if(i != 0 && i % 64 == 0) {
			x = 0;
			y += 10;
			std::swap(even_color,odd_color);
		}

		if(i % 2 == 0) {
			pixels[i].set_vals(x,y,PIXEL_WIDTH,PIXEL_HEIGHT,even_color);
		}
		else {
			pixels[i].set_vals(x,y,PIXEL_WIDTH,PIXEL_HEIGHT,odd_color);
		}
		x += 10;

	}
}

void sdl_chip8_window::draw_pixels() {
	for(int i = 0; i < 64*32; ++i) {
		SDL_FillRect(window_surface,&(pixels[i].pixel_rect),pixels[i].pixel_color);
	}
	SDL_UpdateWindowSurface(window);
}

void sdl_chip8_window::randomize_pixels() {
	char r;
	for(int i = 0; i < 64*32; ++i) {
		r = rand() % 2;
		if(r == 0x00) {
			pixels[i].set_color(BLACK);
		}
		else {
			pixels[i].set_color(WHITE);
		}
	}
}