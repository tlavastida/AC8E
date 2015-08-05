/*
sdl_funcs.cpp
implementation of functions in sdl_funcs.h
*/

#include "sdl_funcs.h"
#include <SDL2/SDL.h>

bool setup_sdl(SDL_Window*& w, SDL_Surface*& s, int scr_width, int scr_height, char* str) {
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
