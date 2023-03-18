/* Print NTH line from stream, negative argument makes it print from end. */
/* In case of negative argument program can print line of max BUFSIZ length. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Print NTH line from the beginning of the FP stream. */
void
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
		return;
	}
	while ((c = fgetc(fp)) != EOF && c != '\n') {
		/* TODO(irek): IDK what to do in case of putchar error. */
		putchar(c);
	}
	putchar('\n');
}

/* Print NTH line from the end of the FP stream. */
void
from_end(FILE *fp, int nth)
{
	int i;
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
	}
	free(lines);
}

int
main(int argc, char **argv)
{
	int argv1;
	FILE *fp = stdin;
	if (argc < 2 || (argv1 = atoi(argv[1])) == 0) {
		argv1 = 1; /* Default when no arg or invalid arg */
	}
	if (argc > 2 && (fp = fopen(argv[argc-1], "r")) == NULL) {
		perror("fopen");
		return 1;
	}
	/* Searching from the top is totally different than from the bottom.
	 * First is simple, second requires dynamic memory allocation.
	 * So this is condition that will decide the faith of your memory. */
	if (argv1 > 0) {
		from_beg(fp, argv1);
	} else {
		from_end(fp, argv1*-1); /* Make argv1 positive. */
	}
	if (fp != stdin && fclose(fp) == EOF) {
		perror("fclose");
		return 1;
	}
	return 0;
}
