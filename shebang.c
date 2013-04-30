/* $Id: shebang.c,v 1.1.1.1 2002/01/29 23:26:56 diego Exp $ */
#include <config.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#define _GNU_SOURCE
#include <getopt.h>


#include "shebang.h"

static const int MAXARGS = 100;
static const int COUNT = 1;
static const int DONT_CLOSE_ON_EXEC = 0;


static char buf[MAXBUFSIZE] = { '\0' };
static char sb_cmd[MAXBUFSIZE] = { '\0' };

static const char* SHORT_OPTS = "k:u:V";
static struct option options[] = {
	{"checkargs", required_argument, NULL, 'k'},
	{"usage", required_argument, NULL, 'u'},
	{"version", no_argument, NULL, 'V'},
	{0, 0, 0, 0}
};

void usage(void) {
	fprintf(stderr, "usage: %s { -V|--version | [-k|--checkargs N] "
		"[-u|--usage STR] cmd [OPTIONS]... @! }\n", SHEBANGNAME);
	fprintf(stderr, "*** IMPORTANT NOTICE: %s is not intended for "
		"command line use ***\n", SHEBANGNAME);
	exit(1);
}

void copyright(void) {
	printf("%s Version %s\n", SHEBANGNAME, SB_VERSION);
	printf("Copyright (C) 2001-2013 by Diego Torres Milano "
		"<dtmilano@gmail.com>\n");
	printf("All rights reserved. See COPYING for details.\n");
	exit(0);
}

int main(int argc, char* argv[]) {
	int i;
	char* sb_usage = NULL;
	int sb_checkargs = -1;
	char* sb_script;
	int sb_optind = 1;
	char* sb_eval_name;
	FILE* sb_eval;
	FILE* sb_eval_pipe;
	int sb_opt_argc = MAXARGS;
	char* sb_opt_argv0;
	char* sb_opt_argv[MAXARGS];
	int sb_argc = MAXARGS;
	char* sb_argv0;
	char* sb_argv[MAXARGS];
    const char* sb_input;
	int fd;
	char c;
    int gnabehs = 0;
    int gnabehs_pos_in_argv = -1;

	#ifndef NDEBUG
		printf("shebang:\n");
		printf("arguments:\n");
		for (i=0; i<argc; i++) {
			printf("\t%d:%s\n", i, argv[i]);
		}
	#endif

	if ( argc < 2 ) {
		usage();
	}

	strncpy(buf, argv[0], sizeof(buf));
    for (sb_argc=1; sb_argc < argc; sb_argc++) {
        if (strstr(argv[sb_argc], GNABEHS) != NULL) {
            gnabehs_pos_in_argv = sb_argc;
        }
	    strcat(buf, WSTR);
	    strncat(buf, argv[sb_argc], sizeof(buf));
    }
	if ( str2argv(buf, &sb_opt_argc, sb_opt_argv, &sb_opt_argv0, &gnabehs) < 0 ) {
		Fatal("str2argv");
	}

    /* this is used by getopt_long to stop processing options after first non-option found */
    if (setenv("POSIXLY_CORRECT", "1", 1) == -1) {
        Fatal("Cannot set environment");
    }

	while ( (c = getopt_long(sb_opt_argc, sb_opt_argv, SHORT_OPTS,
			options, NULL)) != -1 ) {

	    #ifndef NDEBUG
			printf("c='%c' optarg='%s'\n", c, optarg);
		#endif
		switch ( c ) {
			case 'k':
                /* checkargs */
				sb_checkargs = strtol(optarg, (char **)NULL, 10);
                fprintf(stderr, "%s: WARNING: Not implemented yet.\n", SHEBANGNAME);
				break;

			case 'u':
                /* usage */
				sb_usage = strdup(optarg);
                fprintf(stderr, "%s: WARNING: Not implemented yet.\n", SHEBANGNAME);
				break;

			case 'V':
                /* version */
				copyright();

            case ':':
			case '?':
				usage();

			default:
				Fatal("getopt (%c)", c);
		}
	}

    if (!gnabehs || gnabehs_pos_in_argv == -1) {
        Fatal("Couldn't find the end of shebang arguments delimited by '%s'", GNABEHS);
    }

	#ifndef NDEBUG
		printf("optind=%d argc=%d sb_argc=%d sb_opt_argc=%d gnabehs_pos_in_argv=%d\n", optind, argc,
            sb_argc, sb_opt_argc, gnabehs_pos_in_argv);
	#endif
    if (optind < sb_opt_argc) {
        #ifndef NDEBUG
            fprintf(stderr, "copying sb_opt_argv[%d] to sb_cmd\n", optind);
            fprintf(stderr, "    %s\n", sb_opt_argv[optind]);
        #endif
		strcpy(sb_cmd, sb_opt_argv[optind]);
		for (i=optind+1; i < sb_opt_argc; i++) {
            #ifndef NDEBUG
                fprintf(stderr, "concatenating i=%d\n", i);
            #endif
            strncat(sb_cmd, WSTR, 1);
			strncat(sb_cmd, sb_opt_argv[i], sizeof(buf));
		}

        if (argc > gnabehs_pos_in_argv+1) {
            sb_input = strdup(argv[gnabehs_pos_in_argv+1]);
            strncat(sb_cmd, WSTR, 1);
            strncat(sb_cmd, sb_input, sizeof(buf));
        }
        else {
            sb_input = "";
        }

        for (i=gnabehs_pos_in_argv+2; i < argc; i++) {
            strncat(sb_cmd, WSTR, 1);
			strncat(sb_cmd, argv[i], sizeof(buf));
        }

        #ifndef NDEBUG
            fprintf(stderr, "We should execute the command in buf='%s'\n", sb_cmd);
            fprintf(stderr, "with input from %s\n", sb_input);
        #endif
    }
    else {
		fprintf(stderr, "%s: ERROR: It seems that you forgot to specify the command to run\n", SHEBANGNAME);
		usage();
	}

	if ( argc < 3 ) {
		usage();
	}

	assert(argc >= 2);

    sb_argc = sizeof(sb_argv)/sizeof(char*);
	if (str2argv(sb_cmd, &sb_argc, sb_argv, &sb_argv0, NULL) < 0) {
        Fatal("str2argv");
    }

    #ifndef NDEBUG
        fprintf(stderr, "execvp '%s", sb_argv0);
        for (i=0; i < sb_argc; i++) {
            fprintf(stderr, " %s", sb_argv[i]);
        }
        fprintf(stderr, "'\n");
    #endif
	execvp(sb_argv0, sb_argv);
	Fatal("exec");
	return 0;
}
