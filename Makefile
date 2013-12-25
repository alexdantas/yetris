# yetris Makefile
# (2013) Alexandre Dantas <eu@alexdantas.net>
#
# This is a rather complex Makefile, sorry about that.
# It supports the following targets:
#
# make all        Builds the package
# make run        Builds and runs the program
# make install    Installs the package on your system
# make uninstall  Uninstalls the package from your system
# make clean      Cleans results of building process
# make dist       Creates source code "tarball"
# make doc        Generates the documentation with doxygen
# make docclean   Removes the documentation
#
# Also, the following commandline arguments customize
# default actions:
#
#  V       Verbose mode, off by default.
#          To turn on for the current command,
#          add `V=1` when calling `make`.
#          To turn on permanently, uncomment the line
#          specified below
#  DESTDIR Installs the package on a custom root directory
#          (other than `/`). For example `DESTDIR=~/`.
#  PREFIX  Installs the package on a custom directory
#          (overwrites root)
#  CFLAGS  Changes the C flags used on compilation
#  CDEBUG  If you wish to build on debug mode, add 'CDEBUG=-g'
#

# Uncomment line below to tun on verbose mode permanently
#V = 1;

# General Info
PACKAGE = yetris
VERSION = 1.6
DATE    = $(shell date "+%b%Y")

# Local source code information
FILES = BUGS ChangeLog COPYING Doxyfile \
        INSTALL.md Makefile README.md TODO

# Install dirs
DESTDIR =
PREFIX  = $(DESTDIR)/usr

EXEC_PREFIX = $(PREFIX)
DATAROOTDIR = $(PREFIX)/share
MANROOT     = $(DATAROOTDIR)/man

MANNUMBER   = 6

BINDIR      = $(EXEC_PREFIX)/games
MANDIR      = $(MANROOT)/man$(MANNUMBER)

MANFILE     = $(PACKAGE).$(MANNUMBER)
MANPAGE     = doc/man/$(MANFILE)

SCORE_FILE  = yetris.scores
CONFIG_FILE = config.ini

# Build info
EXE         = $(PACKAGE)
CDEBUG      = -O2
CFLAGS      = $(CDEBUG) -Wall -Wextra
LDFLAGS     = -lncurses
INCLUDESDIR =
LIBSDIR     =

# All source files
CFILES  = $(shell find src -maxdepth 1 -type f -name '*.c')
OBJECTS = $(CFILES:.c=.o)

DEFINES = -DVERSION=\""$(VERSION)"\"         \
          -DPACKAGE=\""$(PACKAGE)"\"         \
          -DDATE=\""$(DATE)"\"               \
          -DSCORE_FILE=\""$(SCORE_FILE)"\"   \
          -DCONFIG_FILE=\""$(CONFIG_FILE)"\"

# iniparser stuff
INIDIR     = src/iniparser
INI_CFLAGS = -O2 -fPIC -Wall -ansi -pedantic -Wextra
INI_OBJS   = $(INIDIR)/inidictionary.o \
             $(INIDIR)/iniparser.o

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

# Make targets
all: dirs $(EXE)
	# Build successful!

install: all
	# Installing...
	$(MUTE)install -d -m 755 $(BINDIR)
	$(MUTE)install -m 755 bin/$(EXE) $(BINDIR)
	-$(MUTE)cat $(MANPAGE) | sed -e "s|DATE|$(DATE)|g" -e "s|VERSION|$(VERSION)|g" >$(MANFILE)
	$(MUTE)install -d $(MANDIR)
	$(MUTE)install $(MANFILE) $(MANDIR)
	$(MUTE)rm -f $(MANFILE)
	# $(PACKAGE) successfuly installed!

uninstall:
	# Uninstalling...
	$(MUTE)rm -f $(BINDIR)/$(EXE)
	$(MUTE)rm -f $(MANDIR)/$(MANFILE)

purge: uninstall
	# Purging configuration files...
	$(MUTE)rm -f $(MANDIR)/$(MANFILE)

$(EXE): $(OBJECTS) $(INI_OBJS)
	# Linking...
	$(MUTE)$(CC) $(OBJECTS) $(INI_OBJS) -o bin/$(EXE) $(LIBSDIR) $(LDFLAGS)

src/%.o: src/%.c
	# Compiling $<...
	$(MUTE)$(CC) $(CFLAGS) $(CDEBUG) $< -c -o $@ $(DEFINES) $(INCLUDESDIR)

dist: clean $(DISTDIR).tar.gz

$(DISTDIR).tar.gz: $(DISTDIR)
	$(MUTE)tar czf $(DISTDIR).tar.gz $(DISTDIR)
	$(MUTE)rm -rf $(DISTDIR)
	$(MUTE)cp $(DISTDIR).tar.gz ..
	$(MUTE)rm -f $(DISTDIR).tar.gz

$(DISTDIR):
	$(MUTE)mkdir -p $(DISTDIR)/src $(DISTDIR)/doc $(DISTDIR)/bin
	-$(MUTE)cp $(FILES) -t $(DISTDIR)
	-$(MUTE)cp -r src/* $(DISTDIR)/src
	-$(MUTE)cp -r doc/* $(DISTDIR)/doc

dirs:
	@-mkdir -p bin

run: all
	# Running...
	$(MUTE)./bin/$(EXE)

clean:
	# Cleaning files...
	$(MUTE)rm $(VTAG) -f $(OBJECTS) $(INI_OBJS)
	$(MUTE)rm $(VTAG) -f bin/$(EXE)

doc:
	# Generating documentation...
	$(MUTE)doxygen Doxyfile

docclean:
	# Removing documentation...
	-$(MUTE)rm $(VTAG) -rf doc/html

.PHONY: clean doc docclean uninstall dirs

# iniparser stuff

$(INIDIR)/iniparser.o: $(INIDIR)/iniparser.c
	# Compiling iniparser...
	$(MUTE)$(CC) $(INI_CFLAGS) $(INIDIR)/iniparser.c  -c -o $(INIDIR)/iniparser.o

$(INIDIR)/inidictionary.o: $(INIDIR)/dictionary.c
	$(MUTE)$(CC) $(INI_CFLAGS) $(INIDIR)/dictionary.c -c -o $(INIDIR)/inidictionary.o

