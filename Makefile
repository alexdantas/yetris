# yetris Makefile (2013) Alexandre Dantas (kure) <alex.dantas92@gmail.com>
#
# Makefile Targets:
#  make all        Compiles the package
#  make run        Compiles and runs the program
#  make install    Installs the package
#  make clean      Removes the binary and the resulting object files
#  make uninstall  Uninstalls the package
#  make dist       Creates the source code 'tarball' for distribution
#  make doc        Generates the documentation with doxygen
#  make docclean   Removes the documentation
#
# Makefile Commandlines:
#  V       Print all commands as they are called.
#          To turn on for the current make, add 'V=1' on the
#          commandline.
#          To turn on permanently, uncomment the line specified below
#  DESTDIR Installs the package on a custom root directory (other than /)
#          For example 'DESTDIR=~/'.
#  PREFIX  Installs the package on a custom directory (overwrites root)
#  CFLAGS  Changes the C flags used on compilation
#  CDEBUG  If you wish to build on debug mode, add 'CDEBUG=-g'
#

# Uncomment line below to tun on verbose mode permanently
#V	= 1;

SHELL	= /bin/sh

# General Info
PACKAGE = yetris
VERSION = 1.6
DATE	= Feb2013

# Local source code information
LBIN    = bin
LOBJ    = obj
LDOC    = doc
LSRC    = src
LFILES  = BUGS ChangeLog COPYING Doxyfile INSTALL Makefile README TODO

# Install
DESTDIR =
PREFIX	= $(DESTDIR)/usr/local

EXEC_PREFIX = $(PREFIX)
DATAROOTDIR = $(PREFIX)/share
MANDIR      = $(DATAROOTDIR)/man

BINDIR	= $(EXEC_PREFIX)/$(LBIN)
MAN6DIR = $(MANDIR)/man6

# Package configuration files
# Default scores file: /var/games/yetris.scores
SCORE_FILE  = yetris.scores
SCOREDIR    = $(DESTDIR)/var/games
SCORE_PATH  = $(SCOREDIR)/$(SCORE_FILE)
# Default config file: /home/<user>/.yetrisrc.ini
CONFIG_FILE = .yetrisrc.ini
CONFIGDIR   = $(HOME)
CONFIG_PATH = $(CONFIGDIR)/$(CONFIG_FILE)

# Compiling information
CC	        = gcc
EXE	        = yetris
CDEBUG	    =
CFLAGS	    = $(CDEBUG) -Wall -Wextra -O2
LIBS	    = -lncurses
INCLUDESDIR =
LIBSDIR     =
OBJ        = obj/engine.o    obj/piece.o   \
              obj/game.o      obj/main.o    \
              obj/timer.o     obj/board.o   \
              obj/block.o     obj/globals.o \
              obj/arguments.o obj/config.o  \
              obj/hscore.o

MANFILE     = $(PACKAGE).6.gz
MANPAGE     = $(LDOC)/man/$(MANFILE)

DEFINES	= -DVERSION=\"$(VERSION)\"                 \
              -DDATE=\"$(DATE)\"                       \
              -DSCORE_PATH=\"$(SCORE_PATH)\"           \
              -DDEFAULT_CONFIG_FILE=\"$(CONFIG_PATH)\"

# iniparser stuff
INIDIR     = src/iniparser
INI_CFLAGS = -O2 -fPIC -Wall -ansi -pedantic -Wextra
INI_OBJS   = obj/iniparser.o obj/inidictionary.o

# Distribution tarball
TARNAME = $(PACKAGE)
DISTDIR = $(TARNAME)-$(VERSION)

# Verbose mode check
ifdef V
MUTE =
VTAG = -v
else
MUTE = @
endif

ifdef DESTDIR
ROOT = -
else
ROOT =
endif

ifdef DEBUG
CDEBUG = -D_YETRIS_DEBUG
else
CDEBUG =
endif

#############################################################################
# Make targets
all: dirs $(EXE)
	@echo "* Build successful!"

install: all
	@echo "* Installing..."
	$(MUTE)install -d $(SCOREDIR)
	$(MUTE)install -d --mode=755 $(BINDIR)
	$(MUTE)install --mode=755 $(LBIN)/$(EXE) $(BINDIR)
#	$(ROOT)$(MUTE)chown root:games $(BINDIR)/$(EXE)
	$(ROOT)$(MUTE)chmod u+s $(BINDIR)/$(EXE)
	-$(MUTE)chown root:games $(BINDIR)/$(EXE)
	-$(MUTE)chmod u+s $(BINDIR)/$(EXE)
	$(MUTE)install -d $(MAN6DIR)
	$(MUTE)install $(MANPAGE) $(MAN6DIR)
	@echo
	@echo "* $(PACKAGE) successfuly installed!"

uninstall:
	@echo "* Uninstalling..."
	$(MUTE)rm -f $(BINDIR)/$(EXE)

purge: uninstall
	@echo "* Purging configuration files..."
	$(MUTE)rm -f $(SCORE_PATH)
	$(MUTE)rm -f $(MAN6DIR)/$(MANFILE)

$(EXE): $(OBJ) $(INI_OBJS)
	@echo "* Linking..."
	$(MUTE)$(CC) $(OBJ) $(INI_OBJS) -o $(LBIN)/$(EXE) $(LIBSDIR) $(LIBS)

$(LOBJ)/%.o: $(LSRC)/%.c
	@echo "* Compiling $<..."
	$(MUTE)$(CC) $(CFLAGS) $(CDEBUG) $< -c -o $@ $(DEFINES) $(INCLUDESDIR)

dist: $(DISTDIR).tar.gz

$(DISTDIR).tar.gz: $(DISTDIR)
	$(MUTE)tar czf $(DISTDIR).tar.gz $(DISTDIR)
	$(MUTE)rm -rf $(DISTDIR)
	$(MUTE)cp $(DISTDIR).tar.gz ..
	$(MUTE)rm -f $(DISTDIR).tar.gz

$(DISTDIR):
	$(MUTE)mkdir -p $(DISTDIR)/$(LSRC) $(DISTDIR)/$(LDOC)
	$(MUTE)mkdir -p $(DISTDIR)/$(LBIN) $(DISTDIR)/$(LOBJ)
	-$(MUTE)cp $(LFILES) -t $(DISTDIR)
	-$(MUTE)cp -r $(LSRC)/* $(DISTDIR)/$(LSRC)
	-$(MUTE)cp -r $(LBIN)/* $(DISTDIR)/$(LBIN)
	-$(MUTE)cp -r $(LDOC)/* $(DISTDIR)/$(LDOC)
# Not copying git info - unnecessary MBs!
#	-$(MUTE)cp -r .git $(DISTDIR)/

# Phony targets
dirs:
	@-mkdir -p $(LOBJ) $(LBIN)

run: all
	@echo "* Running..."
	$(MUTE)./$(LBIN)/$(EXE)

clean:
	@echo "* Cleaning files..."
	$(MUTE)rm $(VTAG) -f $(LOBJ)/*.o
	$(MUTE)rm $(VTAG) -f $(LBIN)/*

doc:
	@echo "* Generating documentation..."
	$(MUTE)doxygen Doxyfile

docclean:
	@echo "* Removing documentation..."
	-$(MUTE)rm $(VTAG) -rf $(LDOC)/html

.PHONY: clean doc docclean uninstall dirs

#------------------------------------------------------------------------------
# iniparser stuff

obj/iniparser.o: $(INIDIR)/iniparser.c
	@echo "* Compiling iniparser..."
	$(MUTE)$(CC) $(INI_CFLAGS) $(INIDIR)/iniparser.c  -c -o obj/iniparser.o

obj/inidictionary.o: $(INIDIR)/dictionary.c
	$(MUTE)$(CC) $(INI_CFLAGS) $(INIDIR)/dictionary.c -c -o obj/inidictionary.o

