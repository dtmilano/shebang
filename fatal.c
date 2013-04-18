#include <config.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "shebang.h"

extern char emesgstr[];
void my_perror(void);

void Fatal(char* fmt, ...)
{
	va_list argptr;

	#ifndef NDEBUG
		fprintf(stderr,"fatal called\n");
	#endif

	va_start(argptr, fmt);
	vsprintf(emesgstr, fmt, argptr);
#ifndef NDEBUG
	fprintf(stderr, "fatal: emesgstr='%s'\n", emesgstr);
#endif
	va_end(argptr);

	my_perror();

#ifndef NDEBUG
	fprintf(stderr, "fatal: emesgstr='%s'\n", emesgstr);
#endif

	fprintf(stderr,"%s: ERROR %s.\n", SHEBANGNAME, emesgstr);

	exit(1);
}

