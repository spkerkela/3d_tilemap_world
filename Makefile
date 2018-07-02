CC=cc
C_FLAGS=-Wall -std=c99
#SDL=SDL2
#FRAMEWORKS=-framework SDL2
default: all
world : main.c 
	clang-format -i main.c
	$(CC) main.c -o world $(C_FLAGS)
all: world

clean:
	rm world
