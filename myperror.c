/*
 * from utils 2.0
 */

#include <config.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

char emesgstr[255] = "";

void my_perror(void)
{
	register int len;
	char _errnbr[10];
	char* p = NULL;

	if ( errno != 0 ) {
		if ( errno > 0 && errno <= sys_nerr )
			sprintf(_errnbr, " (%d)", errno);
		else
			*_errnbr = '\0';
		p = strerror(errno);

		len = strlen(emesgstr);
		sprintf(emesgstr + len, ".\n%s%s", p, _errnbr);
		}

}

