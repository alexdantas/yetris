# yetris Makefile for Windows
# (2013) Alexandre Dantas (kure) <alex.dantas92@gmail.com>
# Simply go 'make'

PACKAGE = yetris
VERSION = 1.5
DATE	= Feb2013

# Configuration and score
SCORE_PATH  = ./scores.bin
CONFIG_PATH = ./config.ini

# Compiling information
CC	    = gcc
EXE	    = yetris.exe
CFLAGS	    = -Wall -Wextra -O2
LIBS	    = -lcurses -lgw32c -lole32 -luuid -lwsock32

# These dirs contains the windows version of common gnu libs
INCLUDESDIR = -I.\include                  \
              -IC:\GnuWin32\include        \
              -IC:\GnuWin32\include\glibc
LIBSDIR     = -LC:\GnuWin32\lib -LC:\MinGW\lib

SOURCE/OBJ         = source/obj/engine.o    source/obj/config.o  \
              source/obj/game.o      source/obj/main.o    \
              source/obj/timer.o     source/obj/board.o   \
              source/obj/block.o     source/obj/globals.o \
              source/obj/piece.o

DEFINES	= -DVERSION=\"$(VERSION)\"       \
          -DDATE=\"$(DATE)\"             \
          -DSCORE_PATH=\"$(SCORE_PATH)\" \
          -DDEFAULT_CONFIG_FILE=\"$(CONFIG_PATH)\"

# iniparser stuff
INIDIR     = source/iniparser
INI_CFLAGS = -O2 -Wall -ansi -pedantic -Wextra #-fPIC 
INI_SOURCE/OBJS   = source/obj/iniparser.o source/obj/inidictionary.o


# Make targets
all: icon $(EXE)
	@echo "* Build successful!"

$(EXE): $(SOURCE/OBJ) $(INI_SOURCE/OBJS)
	@echo "* Linking..."
	@$(CC) $(SOURCE/OBJ) $(INI_SOURCE/OBJS) source/obj/icon.res -o $(EXE) $(LIBSDIR) $(LIBS)

source/obj/%.o: source/%.c
	@echo "* Compiling $<..."
	@$(CC) $(CFLAGS) $< -c -o $@ $(DEFINES) $(INCLUDESDIR)

clean:
	@rm source/obj/* -f

# Make the .exe have an icon
icon:
	@windres icon.rc -O coff -o source/obj/icon.res

#------------------------------------------------------------------------------
# iniparser stuff

source/obj/iniparser.o: $(INIDIR)/iniparser.c
	@echo "* Compiling iniparser..."
	@$(CC) $(INI_CFLAGS) $(INIDIR)/iniparser.c  -c -o source/obj/iniparser.o

source/obj/inidictionary.o: $(INIDIR)/dictionary.c
	@$(CC) $(INI_CFLAGS) $(INIDIR)/dictionary.c -c -o source/obj/inidictionary.o
