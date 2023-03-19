/* Print NTH line from stream. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Print NTH line from the beginning of the FP stream.
 * Return non zero value when nothing was printed. */
int
from_beg(FILE *fp, int nth)
{
	int c;
	nth--;
	while (nth > 0 && ((c = fgetc(fp)) != EOF)) {
		if (c == '\n') {
			nth--;
		}
	}
	if (c == EOF) {
		return 1;
	}
	while ((c = fgetc(fp)) != EOF && c != '\n') {
		/* TODO(irek): IDK what to do in case of putchar error. */
		putchar(c);
	}
	putchar('\n');
	return 0;
}

/* Print NTH line from the end of the FP stream.
 * Return non zero value when nothing was printed. */
int
from_end(FILE *fp, int nth)
{
	int i, res = 1;
	char **lines = malloc(sizeof(char*)*nth);
	for (i = 0; i < nth; i++) {
		lines[i] = malloc(BUFSIZ);
		memset(lines[i], 0, BUFSIZ); /* Not necessary, but anyway */
	}
	/* Read FP stream until the end storing NTH number of last
	 * lines in memory. */
	for (i = 0; fgets(lines[i%nth], BUFSIZ-1, fp); i++);
	if (i >= nth) {
		/* TODO(irek): IDK what to do in case of fputs error. */
		fputs(lines[(i-nth)%nth], stdout);
		res = 0;
	}
	free(lines);
	return res;
}

int
main(int argc, char **argv)
{
	int argv1 = 0, res;
	FILE *fp = stdin;
	if (argc > 1 &&
	    (!strncmp(argv[1], "--h", 3) ||
	     !strncmp(argv[1],  "-h", 2))) {
		fprintf(stderr,
			"usage: %s [[-][+]n] [file]\n\n"
			"\tn\tLine number to print, default to -1.\n"
			"\t\tNegative number prints from bottom.\n"
			"\tfile\tInput file, default to stdin.\n\n"
			"examples:\n\n"
			"\t$ cat input | nth     # Print last line\n"
			"\t$ tail input | nth 2  # Print second line\n"
			"\t$ nth -3 input        # Third from the bottom\n",
			argv[0]);
		return 1;
	}
	if (argc > 2) {
		fp = fopen(argv[argc-1], "r");
	}
	if (argc > 1) {
		argv1 = atoi(argv[1]);
		if (fp == stdin && argv1 == 0) {
			fp = fopen(argv[argc-1], "r");
		}
	}
	if (fp == NULL) {
		perror("fopen");
		return 1;
	}
	if (argv1 == 0) {
		argv1 = -1; /* By default print last line. */
	}
	/* Searching from the top is totally different than from the bottom.
	 * Only second requires dynamic memory allocation. */
	res = argv1 > 0 ? from_beg(fp, argv1) : from_end(fp, argv1*-1);
	if (fp != stdin && fclose(fp) == EOF) {
		perror("fclose");
		return 1;
	}
	return res;
}
