
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
		{"version",   no_argument, NULL, 'u'},
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
	printf("yetris Help\n"
	       "This is an example of a Help text.\n"
		   "If you want to see more info, use 'yetris --usage'\n"
	       "\n");
}

void print_usage ()
{
	printf("yetris Usage\n\n"
	       "	-c, --center  Center the game on screen.\n"
	       "	-h, --help    Display help.\n"
	       "	-u, --usage   Display this text.\n"
	       "\n");
}

