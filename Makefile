# Yeah

all:
	gcc main.c arguments.c -o yetris -lncurses

run: all
	./yetris

