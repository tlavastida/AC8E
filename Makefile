CC=g++
FLAGS=-std=c++11 -Wall
INCLUDE=-I ./include/
SRC_DIR=./src/
LIBS=-lSDL2

all: main

main: main.o chip8.o
	$(CC) -o main main.o chip8.o $(FLAGS) $(INCLUDE) $(LIBS)

main.o: $(SRC_DIR)main.cpp
	$(CC) -c $(SRC_DIR)main.cpp $(FLAGS) $(INCLUDE) $(LIBS)

chip8.o: $(SRC_DIR)chip8.cpp ./include/chip8.h
	$(CC) -c $(SRC_DIR)chip8.cpp $(FLAGS) $(INCLUDE)

cleanup:
	rm *.o main
