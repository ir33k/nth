/* Print N line from file or stdin. */

#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
	int i;			/* For line index from argv[1] */
	FILE *fp = stdin;	/* Read from stdin by default */
	char line[BUFSIZ], *res;

	/* TODO(irek): When negative number is passed then print line
	 * from bottom. */
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
		perror("fopen");
		return 1;
	}
	/* TODO(irek): Hmm, maybe there is better way to navigate file
	 * by line numbers other than reading the content. */
	while (i-- && (res = fgets(line, BUFSIZ, fp)));
	if (res) {
		fputs(line, stdout);
	}
	if (fp != stdin && fclose(fp) == EOF) {
		perror("fclose");
		return 1;
	}
	return res ? 0 : 1;
}
