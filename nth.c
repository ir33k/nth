/* Print N line from file or stdin. */

#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
	int i;			/* For line index from argv[1] */
	FILE *fp = stdin;	/* Read from stdin by default */
	char line[BUFSIZ], *res;

	if (argc < 2) {
		fprintf(stderr,
			"Print N line from file or stdin.\n\n"
			"usage: %s n [file]\n\n"
			"\tn\tLine number to print.\n"
			"\tfile\tPath to file, use stdin by default.\n",
			argv[0]);
		return 1;
	}
	i = atoi(argv[1]);
	if (argc > 2 && (fp = fopen(argv[argc-1], "rb")) == NULL) {
		fprintf(stderr, "fopen: ");
		perror(NULL);
		return 1;
	}
	while (i-- && (res = fgets(line, BUFSIZ, fp)));
	if (res) {
		fputs(line, stdout);
	}
	if (fp != stdin && fclose(fp) == EOF) {
		fprintf(stderr, "fclose: ");
		perror(NULL);
		return 1;
	}
	return res ? 0 : 1;
}
