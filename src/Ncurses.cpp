#include "Ncurses.hpp"

bool Ncurses::init()
{
	initscr();
	// TODO check for failing

	cbreak();    // Character input doesnt require the <enter> key anymore
	curs_set(0); // Makes the blinking cursor invisible
	noecho();    // Wont print the keys received through input
	nodelay(stdscr, TRUE); // Wont wait for input
	keypad(stdscr, TRUE);  // Support for extra keys (life F1, F2, ... )
	refresh();   // Refresh the layout (prints whats in the layout bu
	return true;
}

void Ncurses::exit()
{
	erase();
	refresh();
	endwin();
}

void Ncurses::delay_ms(int delay)
{
	napms(delay);
}

