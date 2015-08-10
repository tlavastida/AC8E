/*
sdl_funcs.h

functions and structures that deal with sdl

created by Thomas Lavastida - 2015

*/

#ifndef SDL_FUNCS_H
#define SDL_FUNCS_H

#include <SDL2/SDL.h>
#include <iostream>
#include "chip8.h"

//structs and classes

struct chip8_pixel {
	SDL_Rect pixel_rect;
	Uint32 pixel_color;

	void set_vals(int x, int y, int w, int h, Uint32 c) {
		pixel_rect.x = x;
		pixel_rect.y = y;
		pixel_rect.w = w;
		pixel_rect.h = h;
		pixel_color = c;
	}

	void set_color(Uint32 c) {
		pixel_color = c;
	}
};


class sdl_chip8_window {

private:
	Uint32 BLACK, WHITE;		//The only colors we care about for this project

	SDL_Window* window;
	SDL_Surface* window_surface;

	chip8_pixel pixels[64*32];

	SDL_Event event;

	bool setup_sdl(SDL_Window*& w, SDL_Surface*& s, int scr_width, int scr_height, const char* str);
	void setup_pixels();

public:
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 320;
	const int PIXEL_WIDTH = 10;
	const int PIXEL_HEIGHT = 10;

	bool quit;

	//sdl_chip8_window();
	sdl_chip8_window(std::string str);
	~sdl_chip8_window();

	void draw_pixels();
	void set_pixels(const chip8& c);
	void randomize_pixels();

	void handle_events(chip8& c);
};


#endif