# yetris

Customizable Tetris(tm) with textual interface.

![screenshot](http://yetris.alexdantas.net/images/classic-tetris.png)

## Introduction

yetris is a customizable Tetris(tm) clone for the terminal.
It has some features found on modern adaptations and aims
to please both casual players and Tetris(tm) enthusiasts.

It has full textual interface, check out these features:

* _Colors_ and animated interface;
* _Customizable gameplay_, with several game mode
  possibilities: ghost piece, hold piece, slide left/right,
  _invisible_ and custom initial level and noise (like
  the old B-mode).
* _Customizable interface_: up to 7 next pieces, toggle
  game borders, center screen, modify game _random algorithm_...
* You can change settings on _in-game menus_ and options;
* _Game profiles_: settings are automatically saved and
  restored at startup. _Multple profiles_ supported.
* _Game statistics_: cleared lines, individual and total pieces...

yetris was originally developed in C, now reprogrammed in C++
with ncurses. It runs on pretty much every terminal around here,
but it's limited to POSIX systems.

## Screenshots

![main-menu](http://yetris.alexdantas.net/images/main-menu-without-borders.png)

![profile-menu](http://yetris.alexdantas.net/images/profile-menu.png)

![single-player-menu](http://yetris.alexdantas.net/images/single-player-menu.png)

![options-menu](http://yetris.alexdantas.net/images/options-menu.png)

![b-mode](http://yetris.alexdantas.net/images/b-mode.png)

![help](http://yetris.alexdantas.net/images/help.png)

![pause-menu](http://yetris.alexdantas.net/images/pause-menu.png)

![game-statistics](http://yetris.alexdantas.net/images/statistics.png)

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

## Dependencies

yetris only depends on `ncurses`; it's used to show cute things on the terminal.

Make sure you have the package *ncurses dev*. Note that it's _not_ the default
that comes with your distro.

| Distro         | Installation command              |
| -------------- | --------------------------------- |
| Ubuntu/Debian  | `apt-get install libncurses5-dev` |
| Fedora         | `yum install ncurses-devel`       |
| Arch Linux     | _comes by default_                |

If you know the means to install on other distros, [please tell me][issues].

## Usage

Briefly, the following shell commands should configure,
build and install this package on the default directories:

    $ make
    $ [sudo] make install

If you want to test it before installing, do:

    $ make run

Then, it's simple:

	$ man yetris
	$ yetris --help
	$ yetris

Check file `INSTALL.md` for more info.

## Hacking

There's another `README.md` on the `src/` directory.
In there I try to explain the source and how could you play
with it.

If you want to contribute,
[there's a lot we could do together](https://github.com/alexdantas/yetris/issues)!
Don't hesitate to contact me, any addition is welcome.

The `BUGS` and `TODO` files are nice points to check out
what we could do.

Also, if you're like me and *love* ncurses games, with textual
interface and stuff, the `src/Inteface/*` files could be of
great use. I tried to make them very flexible so you could
just insert them into a project without too much change.

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

Also, some miscellaneous credits:

* Yetris' logo uses the *Crawford2* _Figlet Font_.
* Thanks, *Nicolas Devillard*! Your
  [_iniParser_](http://ndevilla.free.fr/iniparser/) helped me
  a lot!
* I'm also using
  [clibs/commander](https://github.com/clibs/commander),
  simples CLI argument parser I've ever seen.
* _Emacs_, _Awesome_ and _Arch Linux_, the Ultimate Triforce.
* [The OneUps](https://theoneups.bandcamp.com/) kept me awake
  on several nights. Their music is great, specially _Volume 2_!

## License

 yetris - Customizable Tetris(tm) on the console.
 Copyright (C) 2013-2014  Alexandre Dantas (kure)

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

[issues]: https://github.com/alexdantas/yetris/issues

