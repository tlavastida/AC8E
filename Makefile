CC=g++
FLAGS=-std=c++11 -Wall
INCLUDE=-I ./include/
LIBS=-lSDL2

SRC_DIR=./src/
INCLUDE_DIR=./include/

all: main

main: main.o chip8.o sdl_funcs.o
	$(CC) -o main main.o chip8.o sdl_funcs.o $(FLAGS) $(INCLUDE) $(LIBS)

main.o: $(SRC_DIR)main.cpp
	$(CC) -c $(SRC_DIR)main.cpp $(FLAGS) $(INCLUDE) $(LIBS)

chip8.o: $(SRC_DIR)chip8.cpp $(INCLUDE_DIR)chip8.h
	$(CC) -c $(SRC_DIR)chip8.cpp $(FLAGS) $(INCLUDE)

sdl_funcs.o: $(SRC_DIR)sdl_funcs.cpp $(INCLUDE_DIR)sdl_funcs.h
	$(CC) -c $(SRC_DIR)sdl_funcs.cpp $(FLAGS) $(INCLUDE) $(LIBS)

cleanup:
	rm *.o main
