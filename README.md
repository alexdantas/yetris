# yetris README

Customizable Tetris(tm) on the console.

![screenshot](http://yetris.alexdantas.net/images/1gameplay.png)

## Introduction

yetris is a customizable Tetris(tm) clone for the console.
It aims to follow modern implementations with lots of features
and please both casual players and Tetris(tm) enthusiasts.

It has full textual interface, with colors and many features:

* Customizable by commandline arguments and config file.
* High score.
* Up to 6 next pieces.
* Hold, Combo and back-to-back sequences.
* Scoring system compliant to the (conjectured)
  *Tetris(tm) Guideline*.

yetris is developed on C and ncurses, running on (most) Linux
terminals.
It currently lacks some expected features but is on active
development.

Also, the source code is clean and commented, allowing much better
understanding and easing feature-implementation.

## Controls

| Key         | Action                                    |
| ----------- | ----------------------------------------- |
| Left, Right | Controls the piece                        |
| Down        | Soft-drop                                 |
| Space       | Hard-drop                                 |
| c           | Holds the piece                           |
| z, x        | Rotates piece counter-clockwise/clockwise |
| p           | Pauses the game                           |
| q           | Quits the game at any time                |
| r           | Restart game                              |
| h           | Show help window                          |
| F2          | Switch statistics                         |
| F3          | Show high scores                          |
| F5          | Refresh game based on config file         |

To see more info, run `yetris --help` and `yetris --usage`.

## Installation

Briefly, the following shell commands should configure,
build and install this package on the default directories:

    $ make
    $ [sudo] make install

If you want to test the game before installing, do:

    $ make run

To see how things end up being installed on your system,
see file `INSTALL.md`.

## Dependencies

To build yetris from source, you need the
*ncurses developer library* `libncurses5-dev (>= 5.7)`.

Note that this is **not** the one that comes by default
with your distribution.

To get it, use your favorite package manager.
An example on apt-based systems:

    $ apt-get install ncurses-dev

## Main Directory Structure

| File     | What it is                                                       |
| -------- | ---------------------------------------------------------------- |
| README   | General game information                                         |
| INSTALL  | Installation instructions                                        |
| COPYING  | Copyright and warranty info                                      |
| Doxyfile | Doxygen file for generating the source documentation             |
| Makefile | Instructions to the `make` program (build and install the game)  |
| TODO     | Work that needs to be done or ideas for future versions          |
| BUGS     | Known and solved bugs on the game                                |
| bin/     | Location of the executable generated after compilation           |
| doc/     | The documentation files (explaining the source code) and manpage |
| obj/     | Location of resulting object files after compilation             |
| src/     | All of the source code files                                     |
| doc/index.html  |  Complete source code documentation                       |
| doc/yetris.6.gz |  The manpage                                              |

## Hacking

I'll add more info about development here soon, so stay tuned.

## Contact

Hello there, I'm Alexandre Dantas (kure)!

Thanks for downloading and playing this game.
I'm glad you had interest on this project.

You can send me comments, bugs, ideas or anything else by email.
And if you have time, please visit my blog!

* My email:           `eu @ alexdantas.net`
* My homepage:        http://www.alexdantas.net/
* yetris main page:   http://yetris.alexdantas.net/
* yetris on GitHub:   https://github.com/alexdantas/yetris/

I'd appreciate any commentary - even if it's just
*"Hello, I play your game!"*.

## Credits

Huge thanks for all the people that contributed to the project
(in order of appearance):

* [Karfield](http://www.github.com/karfield) fixed a great
  mistake regarding `typedefs` and inclusions.
  Thanks to him, *yetris now works on MacOsX*!
* [Rodrigo Rebello](https://github.com/rrebello) fixed some
  resource leaks and optimized the game by removing
  unnecessary `fflush()` calls.
* [Damon McDougall](https://github.com/dmcdougall) pointed out
  [a serious issue](https://github.com/alexdantas/yetris/issues/1)
  on the building process. Thanks to him *yetris can be installed
  on BSD*!

## License

 yetris - Customizable Tetris(tm) on the console.
 Copyright (C) 2013  Alexandre Dantas (kure)

 yetris is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

[![githalytics.com alpha](https://cruel-carlota.pagodabox.com/fde5a2c1dd787040f5121109b5451879 "githalytics.com")](http://githalytics.com/alexdantas/yetris)

