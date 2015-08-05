/*
sdl_funcs.h

functions and structures that deal with sdl
*/

#ifndef SDL_FUNCS_H
#define SDL_FUNCS_H

#include <SDL2/SDL.h>

struct chip8_pixel {
	SDL_Rect pixel_rect;
	Uint32 pixel_color;

	chip8_pixel(int x, int y, int w, int h, Uint32 c) {
		pixel_rect.x = x;
		pixel_rect.y = y;
		pixel_rect.w = w;
		pixel_rect.h = h;
		pixel_color = c;
	}
};

bool setup_sdl(SDL_Window*& w, SDL_Surface*& s, int scr_width, int scr_height, char* str);

#endif