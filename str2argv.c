/*
 * from utils 2.0
 */

#include <config.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>

#include "shebang.h"

#define MAXSTRSIZE 5120
int str2argv(const char* str, int* argc, char* argv[], char** cmd, int* gnabehs)
{
	/*
	 * str2argv:
	 * converts the string str in a comand name cmd, an argument count
	 * argc and the array argv containing the tokens (words) found in str
	 * delimiteds by ' ' (space), '\t' (tab) or '\n' (new line).
	 * str2argv expects at input the value in argc being the maximum space allocated by
	 * the array pointed to by argv.
     * str2argv also looks for GNABEHS string and if found, stores its position in
     * gnabehs.
     * If some argument in argv contains and environment variable it will be replaced by
     * its value.
     * 
     *
	 * WARNING: argv is an in/out parameter and should be allocated
	 * before the call to str2argv.
	 *
	 * params:
	 * str		the command string
	 * argc		input/output  -> the size of input argv
	 * argv     output -> the splitted array
	 * cmd		output -> the command name
     * gnabehs  input  -> can be NULL and no value will be stored
     *          output -> the position of GNABEHS in argv
	 */
	const char* delim = " \t\n";
	static char _str[MAXSTRSIZE];
	int i = 1;
	char* p;

    // default to 0 (false), GNABEHS not found
    if (gnabehs != NULL) {
        *gnabehs = 0;
    }

	#ifndef NDEBUG
		fprintf(stderr, "str2argv: str='%s' *argc=%d argv[]=%p cmd=%p\n", str, *argc, argv, cmd);
	#endif

	if ( strlen(str) > MAXSTRSIZE )
		return -2;

    if ( argc == NULL || argv == NULL || cmd == NULL ) {
        return -2;
    }

    /* find the quotes and preserve whitespace inside them */
    char* s = (char*)str;
	while ( (p = strpbrk(s, "'\"")) ) {
		char d = *p;
		char* q;
		char* r;

        #ifndef NDEBUG
            fprintf(stderr, "while: p='%s'\n", p);
        #endif
		if ( (q = strchr(p+1, d)) == NULL ) {
			/* unbalanced quotes */
			return -3;
		}
		for ( r = p+1; r < q; r++ ) {
			switch ( *r ) {
				case ' ':
					*r = '\x1d';
					break;

				case '\t':
					*r = '\x1e';
					break;

				case '\n':
					*r = '\x1f';
					break;
			}
		}

        s = q + 1;
	}

	if ( strpbrk(str, delim) ) {
		#ifndef NDEBUG
			fprintf(stderr, "str2argv: str='%s' contains delimiters\n", str);
		#endif

		strcpy(_str, str);
		*cmd = strtok(_str, delim);
		#ifndef NDEBUG
			fprintf(stderr, "str2argv: *cmd='%s'\n", *cmd);
		#endif
		argv[0] = basename(*cmd);
		#ifndef NDEBUG
			fprintf(stderr, "str2argv: argv[0]='%s'\n", argv[0]);
		#endif

        i = 1;
		while (i < *argc) {
            const char* _tok = strtok(NULL, delim);
            if ( _tok == NULL) {
                break;
            }
            if (strncmp(GNABEHS, _tok, strlen(GNABEHS)) == 0) {
                if (gnabehs != NULL) {
                    *gnabehs = i;
                }
                break;
            }

            char* var = NULL;
            /* the '$' can be in any position inside the string */
            if ((var = strchr(_tok, '$')) != NULL) {
                char varname[MAXBUFSIZE];
                var++;
                char* varend;
                int v;
                for (varend=var, v=0; *varend; varend++, v++) {
                    char c = *varend;
                    if (c != '_' && (c < '0' || c > '9') && (c < 'A' || c > 'Z') && (c < 'a' || c > 'z')) {
                        break;
                    }
                    varname[v] = *varend;
                }
                varname[v] = 0;
                #ifndef NDEBUG
                    fprintf(stderr, "str2argv: variable='%s'\n", varname);
                    fprintf(stderr, "str2argv: varend='%s'\n", varend);
                #endif
                char buf[MAXBUFSIZE] = { '\0' };
                char* val = getenv(varname);
                #ifndef NDEBUG
                    fprintf(stderr, "str2argv: getenv val=%s\n", val);
                #endif
                if (val != NULL) {
                    strcpy(buf, val);
                }
                else {
                    fprintf(stderr, "%s: WARNING: %s not found in env\n", SHEBANGNAME, varname);
                }
                strcat(buf, varend);
                argv[i] = strdup(buf);
            }
            else {
                argv[i] = (char*)_tok;
            }
			#ifndef NDEBUG
				fprintf(stderr, "str2argv: %d: strtok = '%s'\n", i, argv[i]);
			#endif
            i++;
        }

		char* _tok = strtok(NULL, delim);
		if ( i == *argc && _tok ) {
			#ifndef NDEBUG
				fprintf(stderr, "str2argv: returning -1: i=%d _tok='%s'\n", i, _tok);
			#endif
			return -1;
		}

        argv[i] = NULL;
		*argc = i;
		}
	else {
		*cmd = (char*) str;
		argv[0] = basename((char*)str);
		argv[1] = NULL;
		*argc = 1;
		#ifndef NDEBUG
			fprintf(stderr, "str2argv: cmd = %s\n", *cmd);
		#endif
	}
	
	#ifndef NDEBUG
		fprintf(stderr, "str2argv: setting *argc = %d\n", *argc);
	#endif

    /* revert the whitespaces preserved inside quotes */
	for ( i=0; i < *argc; i++ ) {
		char* r;
		#ifndef NDEBUG
			fprintf(stderr, "str2argv: reverting whitespace in argv[%d]\n", i);
		#endif
		for ( r=argv[i]; *r != '\0'; r++ ) {
			switch ( *r ) {
				case '\x1d':
					*r = ' ';
					break;

				case '\x1e':
					*r = '\t';
					break;

				case '\x1f':
					*r = '\n';
					break;
			}
		}

	}

    #ifndef NDEBUG
        fprintf(stderr, "str2argv: returning argc=%d\n", *argc);
    #endif
	return *argc;
}

