# Yeah

all: main.c
	gcc -g main.c arguments.c -o yetris -lncurses

run: all
	./yetris

