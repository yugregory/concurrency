CC = clang++
CFLAGS = -latomic -std=c++17 

run: main.cpp
	$(CC) $^ $(CFLAGS) -o $@

