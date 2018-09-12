CC = g++
CFLAGS = -ldl -latomic -Wall -g -std=c++17 

run: main.cpp
	$(CC) $^ $(CFLAGS) -o $@

