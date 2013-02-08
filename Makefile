# Makefile for yetris - your tetris!

# Variables
EXEC   = yetris
CFLAGS = #-Wall -Wextra
OBJ    = obj/block.o obj/piece.o  obj/game.o obj/main.o \
         obj/board.o obj/engine.o obj/arguments.o obj/globals.o

# Build instructions
all: dirs ${EXEC}
	@echo "Done!"

${EXEC}: ${OBJ}
	@echo "Linking..."
	@gcc $(OBJ) -o bin/$(EXEC) -lncurses

obj/%.o: src/%.c
	@echo "Compiling $<..."
	@gcc $(CFLAGS) $< -c -o $@

# Helper methods
run: all
	@bin/${EXEC}

clean:
	@rm -vf obj/*

dirs:
	@mkdir -p obj bin src doc

