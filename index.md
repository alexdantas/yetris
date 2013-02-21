---
layout: page
title: yetris
tagline: Customizable Tetris(tm) on console
---
{% include JB/setup %}

<a href="https://github.com/alexdantas/yetris/zipball/master"
   style="margin-right: 50px">
Download .zip
</a>
<a href="https://github.com/alexdantas/yetris/tarball/master"
   style="margin-right: 50px">
Download .tar.gz
</a>
<a href="https://github.com/alexdantas/yetris"
   style="margin-right: 50px">
View on GitHub
</a>

## Welcome!
From the same creator of [nSnake](http://sourceforge.net/projects/nsnake/),
_yetris_ is a customizable Tetris(tm) clone for the terminal.
It aims to follow modern implementations with lots of features
and please both casual players and Tetris(tm) enthusiasts.

It has full textual interface, with colors and many features:
 * Customizable by commandline arguments and config file;
 * High score;
 * Up to 6 next pieces;
 * Hold, combo and back-to-back sequences;
 * Scoring system compliant to the (conjectured) Tetris(tm) Guideline;

_yetris_ is developed on C and ncurses, running on (most) Linux terminals.
There's a beta port to Windows, using pdcurses. It currently lacks some
expected features, but it's on active development.

Also, the source code is clean and commented, allowing much better understanding
and easing feature-implementation.

## Downloads
Aside from GitHub, you can get this project on many other source code hosting
websites:
 * [Download from Sourceforge (stable releases)](https://sourceforge.net/projects/yetris-yetris/files/latest/download)
 * [yetris on Freecode](https://freecode.com/projects/yetris)
 * [yetris on Google Code](https://code.google.com/p/yetris)
 * [yetris on Ohloh.net](https://www.ohloh.net/p/yetris)

## Installation
    $ cd yetris-folder/
    $ make
    $ make install

    # If you want to test the game before installing
    $ make run

Remember to check and edit your `~/.yetrisrc.ini` configuration file!

## Screenshots
![fancy borders](https://sourceforge.net/p/yetris-yetris/screenshot/1gameplay.png)
![on windows xp](https://sourceforge.net/p/yetris-yetris/screenshot/printyetrisXP.PNG)
![old school - no hold, no ghost, one next piece](https://sourceforge.net/p/yetris-yetris/screenshot/2gameplay.png)
![more customizations](https://sourceforge.net/p/yetris-yetris/screenshot/3gameplay.png)
![even more customizations](https://sourceforge.net/p/yetris-yetris/screenshot/4gameplay.png)
![game paused](https://sourceforge.net/p/yetris-yetris/screenshot/5game.png)

## Authors, Support and Contact
Thank you for having interest on this project!
You can contact me through the following means:

author: Alexandre Dantas (@alexdantas)

mailto: alex.dantas92@gmail.com

homepage: http://www.alexdantas.net/

## Blog
This is also a blog frontend to yetris' development.

### Posts list

<ul class="posts">
  {% for post in site.posts %}
    <li><span>{{ post.date | date_to_string }}</span> &raquo; <a href="{{ BASE_PATH }}{{ post.url }}">{{ post.title }}</a></li>
  {% endfor %}
</ul>

