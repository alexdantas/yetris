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

#include <stdio.h>      /* For printf() */
#include <stdlib.h>     /* For EXIT_SUCCESS and FAILURE */
#include <getopt.h>     /* For getopt_long() */
#include "arguments.h"
#include "globals.h"

/** Handles all the commandline arguments.
 */
void args_handle (int argc, char* argv[])
{
	static struct option options[] =
	{
		{"help",      no_argument, NULL, 'h'},
		{"usage",     no_argument, NULL, 'u'},
		{"center",    no_argument, NULL, 'c'},
		{"no-colors", no_argument, NULL, 'n'},
		/* The last element must be all zeroes */
		{0, 0, 0, 0}
	};
	/* The index of the current option */
	int option_index;
	/* The character for comparison */
	int c = 0;


	/* We keep checking the arguments untill they run out (c == -1) */
	while (c != -1)
	{
		c = getopt_long (argc, argv, "hucn", options, &option_index);

		switch (c)
		{
		case 'h':
			print_help();
			exit(EXIT_SUCCESS);
			break;
		case 'u':
			print_usage();
			exit(EXIT_SUCCESS);
			break;
		case 'c':
			global.screen_center_vertically   = true;
			global.screen_center_horizontally = true;
			break;
		case 'n':
			global.screen_use_colors = false;
			break;

		case '?':
			/* getopt_long() already printed an error message about
			 * unrecognized option, so you'll probably want to
			 * abort the execution now */
			printf("Use 'yetris --help' for more information\n");
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
	       "	-c, --center   Center the game on screen.\n"
	       "	-h, --help     Display help.\n"
	       "	-u, --usage    Display this text.\n"
	       "	-n, --no-color Runs the game without colors.\n");
}

