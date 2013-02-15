/* yetris - Tetris(tm) on the console.
 * Copyright (C) 2013 Alexandre Dantas (kure)
 *
 * This file is part of yetris.
 *
 * yetris is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * homepage: http://www.github.com/alexdantas/yetris/
 * mailto:   alex.dantas92@gmail.com
 */

#include <stdio.h> /* printf() */
#include <stdlib.h> /* EXIT_SUCCESS and FAILURE */
#include <getopt.h> /* getopt_long() */
#include "arguments.h"
#include "globals.h"

/** Handles all the commandline arguments.
 */
void args_handle (int argc, char* argv[])
{
	static struct option options[] =
	{
		/* NULL tells getopt_long to return the short option */
		{"help",      no_argument, NULL, 'h'},
		{"usage",     no_argument, NULL, 'u'},
		{"center",    no_argument, NULL, 'c'},
		{"no-colors", no_argument, NULL, 'n'},
		{"no-ghost",  no_argument, NULL, 'g'},
		{"no-hold",   no_argument, NULL, 'o'},
		{"next",      required_argument, NULL, 'x'},
		/* The last element must be all zeroes */
		{0, 0, 0, 0}
	};
	/* The index of the current option */
	int option_index;
	/* The character for comparison */
	int c = 0;
	/* All short options - place a colon after it if requires argument */
	char short_options[] = "hucngox:";

	/* We keep checking the arguments untill they run out (c == -1) */
	while (c != -1)
	{
		c = getopt_long (argc, argv, short_options, options, &option_index);

		switch (c)
		{
		/* Local functions that print stuff */
		case 'h': print_help();  exit(EXIT_SUCCESS); break;
		case 'u': print_usage(); exit(EXIT_SUCCESS); break;

		/* Game flags */
		case 'n': global.screen_use_colors = false; break;
		case 'g': global.game_has_ghost    = false; break;
		case 'o': global.game_can_hold     = false; break;
		case 'c':
			global.screen_center_vertically   = true;
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

void print_help ()
{
	printf("yetris v" VERSION " Help\n"
	       "Controls:\n"
		   "\tLeft, Right Controls the piece\n"
	       "\tDown        Soft-drop\n"
	       "\tSpace       Hard-drop\n"
	       "\tc           Holds the piece\n"
	       "\tz, x        Rotates the piece counter-clockwise and clockwise\n"
	       "\tq           Quits the game at any time\n"
		   "For more info, use 'yetris --usage'\n");
}

void print_usage ()
{
	printf("yetris v" VERSION " Usage\n"
		   "	yetris [options]\n"
		   "\n"
	       "	-c, --center     Center the game on screen.\n"
	       "	-h, --help       Display help.\n"
	       "	-u, --usage      Display this text.\n"
	       "	-n, --no-color   Runs the game without colors.\n"
	       "	-o, --no-hold    Disable hold.\n"
	       "	-g, --no-ghost   Disable ghost piece.\n"
	       "	-x, --next (num) Show (num) next pieces\n");
}

