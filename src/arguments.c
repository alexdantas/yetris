
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
		{"help",    no_argument, NULL, 'h'},
		{"version", no_argument, NULL, 'u'},
		{"center",  no_argument, NULL, 'c'},
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
		c = getopt_long (argc, argv, "huc", options, &option_index);

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

		case '?':
			/* getopt_long() already printed an error message about
			 * unrecognized option, so you'll probably want to
			 * abort the execution now */
			break;

		case -1:
			/* There were no '-' parameters passed
			 * or all the parameters were processed */
			break;

		default:
			/* Do nothing - i suggest printing the usage and aborting
			 * execution */
			break;
		}
	}

	/* Just in case the user specified more arguments (not options)
	 * than needed, you decide what to do. Here, we just ignore them */
	while (optind < argc)
		optind++;

	/* Goodbye, hope you liked it! */
}

void print_help ()
{
	printf ("* Arguments Help\n");
	printf ("* This is an example of a Help text.\n");
	printf ("* If you want to modify the actions taken by\n");
	printf ("* the arguments module, please change the \n");
	printf ("* args_handle() function.\n");
	printf ("\n");
}

void print_usage ()
{
	printf ("* Arguments Usage\n");
	printf ("* This is an example of a Usage text.\n");
	printf ("* Insert what you wish.\n");
	printf ("\n");
}

