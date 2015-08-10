#Makefile for AC8E

#Created by Thomas Lavastida - 2015

CC=g++
FLAGS=-std=c++11 -Wall
INCLUDE=-I ./include/
LIBS=-lSDL2

SRC_DIR=./src/
INCLUDE_DIR=./include/

all: AC8E

AC8E: main.o chip8.o sdl_funcs.o
	$(CC) -o AC8E main.o chip8.o sdl_funcs.o $(FLAGS) $(INCLUDE) $(LIBS)

main.o: $(SRC_DIR)main.cpp
	$(CC) -c $(SRC_DIR)main.cpp $(FLAGS) $(INCLUDE) $(LIBS)

chip8.o: $(SRC_DIR)chip8.cpp $(INCLUDE_DIR)chip8.h
	$(CC) -c $(SRC_DIR)chip8.cpp $(FLAGS) $(INCLUDE)

sdl_funcs.o: $(SRC_DIR)sdl_funcs.cpp $(INCLUDE_DIR)sdl_funcs.h
	$(CC) -c $(SRC_DIR)sdl_funcs.cpp $(FLAGS) $(INCLUDE) $(LIBS)

cleanup:
	rm *.o AC8E
