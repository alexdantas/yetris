#include "Ncurses.hpp"

int main(int argc, char *argv[])
{
	(void)(argc);
	(void)(argv);

	Ncurses::init();

	Ncurses::delay_ms(1000);

	Ncurses::exit();
	return 0;
}

