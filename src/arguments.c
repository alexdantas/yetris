
#include <stdio.h> /* printf() */
#include <stdlib.h> /* EXIT_SUCCESS and FAILURE */
#include <getopt.h> /* getopt_long() */
#include "arguments.h"
#include "globals.h"
#include "hscore.h"

/** Handles all the commandline arguments.
 */
void args_handle (int argc, char* argv[])
{
	static struct option options[] =
	{
		/* NULL tells getopt_long to return the short option */
		{"help",	  no_argument, NULL, 'h'},
		{"usage",	  no_argument, NULL, 'u'},
		{"version",	  no_argument, NULL, 'v'},
		{"license",	  no_argument, NULL, 'l'},
		{"center",	  no_argument, NULL, 'c'},
		{"no-colors", no_argument, NULL, 'n'},
		{"no-ghost",  no_argument, NULL, 'g'},
		{"no-hold",	  no_argument, NULL, 'o'},
		{"no-fancy",  no_argument, NULL, 'y'},
		{"no-outer",  no_argument, NULL, 'b'},
		{"reset-hscores",  no_argument, NULL, 'r'},
		{"next",	  required_argument, NULL, 'x'},
		/* The last element must be all zeroes */
		{0, 0, 0, 0}
	};
	/* The index of the current option */
	int option_index;
	/* The character for comparison */
	int c = 0;
	/* All short options - place a colon after it if requires argument */
	char short_options[] = "hucngoybvlrx:";

	/* We keep checking the arguments untill they run out (c == -1) */
	while (c != -1)
	{
		c = getopt_long (argc, argv, short_options, options, &option_index);

		switch (c)
		{
		/* Local functions that print stuff */
		case 'h': print_help();	   exit(EXIT_SUCCESS); break;
		case 'u': print_usage();   exit(EXIT_SUCCESS); break;
		case 'v': print_version(); exit(EXIT_SUCCESS); break;
		case 'l': print_license(); exit(EXIT_SUCCESS); break;

		/* Game flags */
		case 'n': global.screen_use_colors	  = false; break;
		case 'y': global.screen_fancy_borders = false; break;
		case 'b': global.screen_show_outer_border = false; break;
		case 'g': global.game_has_ghost		  = false; break;
		case 'o': global.game_can_hold		  = false; break;
		case 'c':
			global.screen_center_vertically	  = true;
			global.screen_center_horizontally = true;
			break;
		case 'x':
			{
			char num = atoi(optarg);

			if ((num >= 0) && (num <= NEXT_PIECES_NO))
				global.game_next_no = atoi(optarg);
			else
			{
				fprintf(stderr, "Invalid number '%s': use something between "
								"1 and %d.\n", optarg, NEXT_PIECES_NO);
				exit(EXIT_FAILURE);
			}
			}
			break;

		case 'r':
			hscore_reset();
			break;

		case '?':
			/* Unexistant option or non-supplied argument.
			 * getopt_long() already printed an error message about
			 * this, so you'll probably want to abort the execution now */
			fprintf(stderr, "Use 'yetris --usage' for more information\n");
			exit(EXIT_FAILURE);
			break;

		case -1:
			/* There were no '-' parameters passed
			 * or all the parameters were processed */
			break;

		default:
			/* Do nothing. I suggest printing the usage and aborting
			 * execution */
			break;
		}
	}

	/* Just in case the user specified more arguments (not options)
	 * than needed, you decide what to do. Here, we just ignore them */
	while (optind < argc)
		optind++;
}

void print_help()
{
	printf("yetris v"VERSION" ("DATE") Help\n"
		   "\n"
		   "yetris is a customizable Tetris(tm) clone for the console.\n"
		   "It aims to follow modern implementations with lots of features\n"
		   "and please both casual players and Tetris(tm) enthusiasts.\n"
		   "\n"
		   "Configuration:\n"
		   "\n"
		   "yetris allow configuration through commandline arguments\n"
		   "and a config file. To see all possible arguments,\n"
		   "use 'yetris --usage'.\n"
		   "\n"
		   "The current config filename is '~/."PACKAGE"/"CONFIG_FILE"'.\n"
		   "All the customizatoins are shown and commented, so go edit it.\n"
		   "Commandline arguments take precedence over the config file.\n"
		   "\n"
		   "Controls:\n"
		   "\n"
		   "Left, Right Controls the piece\n"
		   "Down		Soft-drop\n"
		   "Space		Hard-drop\n"
		   "c			Holds the piece\n"
		   "z, x		Rotates the piece counter-clockwise and clockwise\n"
		   "p			Pauses the game\n"
		   "q			Quits the game at any time\n"
		   "h		   Show help window.\n"
		   "F2		   Switch statistics\n"
		   "F5		   Refresh game based on config file\n");
}

void print_usage()
{
	printf("yetris v"VERSION" ("DATE") Usage\n"
		   "\n"
		   "	yetris [options]\n"
		   "\n"
		   "	-h, --help			Show help about the program\n"
		   "	-u, --usage			Display this text\n"
		   "	-v, --version		Display version and technical information\n"
		   "	-l, --license		Display licensing information\n"
		   "	-c, --center		Center the game on screen\n"
		   "	-n, --no-color		Runs the game without colors\n"
		   "	-o, --no-hold		Disable hold\n"
		   "	-g, --no-ghost		Disable ghost piece\n"
		   "	-y, --no-fancy		Don't show fancy borders\n"
		   "	-b, --no-outer		Don't show outer border on game screen\n"
		   "	-x, --next (num)	Show (num) next pieces\n"
		   "	-r, --reset-hscores Reset all high scores to default\n");
}

void print_version()
{
	printf("yetris v"VERSION" ("DATE")\n"
		   "Copyright (C) 2013 Alexandre Dantas\n"
		   "\n"
		   "This program comes with ABSOLUTELY NO WARRANTY\n"
		   "for warranty details type `yetris --license`.\n"
		   "This is free software, and you are welcome to redistribute it\n"
		   "under certain conditions; type `yetris --license` for license details.\n"
		   "\n"
		   "Mailto:	  eu@alexdantas.net\n"
		   "Homepage: http://yetris.alexdantas.net/\n"
		   "\n");
}

void print_license ()
{
	/* preamble of the GNU GPLv3 */
	printf("yetris - Customizable Tetris(tm) on the console.\n"
		   "Copyright (C) 2013 Alexandre Dantas\n"
		   "\n"
		   "yetris is free software: you can redistribute it and/or modify\n"
		   "it under the terms of the GNU General Public License as published by\n"
		   "the Free Software Foundation, either version 3 of the License, or\n"
		   "any later version.\n"
		   "\n"
		   "This program is distributed in the hope that it will be useful,\n"
		   "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
		   "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
		   "GNU General Public License for more details.\n"
		   "\n"
		   "You should have received a copy of the GNU General Public License\n"
		   "along with this program.  If not, see <http://www.gnu.org/licenses/>.\n"
		   "\n");
}

