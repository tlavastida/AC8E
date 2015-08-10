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

created by Thomas Lavastida - 2015

*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "chip8.h"
#include "sdl_funcs.h"

int process_args(const int& argc, char* argv[], std::string& str);

int main(int argc, char* argv[]) {
	std::string window_title = "AC8E: ";
	if(process_args(argc,argv,window_title) < 0) {
		return 1;
	}


	//create chip8 object and load game
	chip8 my_chip8;
	if(!my_chip8.load_program(argv[1])) {
		std::cout << "failed to load " << argv[1] << ", exiting ...\n";
		return 2;
	}

	sdl_chip8_window chip8_window(window_title);
	//screen setup, do stuff with it

	//main loop
	while(!(chip8_window.quit)) {
		my_chip8.emulate_cycle();
		if(my_chip8.draw_new_frame()) {
			chip8_window.set_pixels(my_chip8);
			chip8_window.draw_pixels();
		}
		//handle events
		chip8_window.handle_events(my_chip8);
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
