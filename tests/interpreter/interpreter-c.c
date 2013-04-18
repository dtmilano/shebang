#include <stdio.h>

int main(int argc, char* argv[]) {
	int i;

	printf("interpreter-c:\n");
	for (i=0; i < argc; i++) {
		printf("\targv[%d]='%s'\n", i, argv[i]);
	}
}
