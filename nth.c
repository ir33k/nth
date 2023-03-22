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
	int i=0, bi=0, res=1;
	size_t len=0, bufsiz=BUFSIZ;
	char *buf[2];
	/* Normally when you need circular buffer it's quite simple.
	 * But normally you know the size of each element in array.
	 * Here I have strings of different length pack together.
	 * Circular buffer is done by switching between 2 buffers. */
	buf[0] = malloc(bufsiz);
	buf[1] = malloc(bufsiz);
	while (fgets(&buf[bi][len], bufsiz-len, fp)) {
		len += strlen(&buf[bi][len]);
		if (buf[bi][len-1] != '\n' || len+1 >= bufsiz) {
			bufsiz *= 2;
			buf[0] = realloc(buf[0], bufsiz);
			buf[1] = realloc(buf[1], bufsiz);
			if (!buf[0] || !buf[1]) {
				perror("realloc");
				return 1;
			}
			continue;
		}
		if (++i == nth) {
			/* Switch to second buffer. */
			bi = bi ? 0 : 1;
			len = 0;
			i = 0;
			res = 0; /* We have at least nth lines in memory. */
			continue;
		}
		len++; /* Separate strings with null. */
	}
	if (res == 0) {
		len = 0;
		/* We always want to read result from other buffer. */
		bi = bi ? 0 : 1;
		/* When "i" is > 0 then we need to skip that many
		 * strings from current buffer.  This happens when
		 * previous buffer was not filled entirly with NTH
		 * number of lines. */
		while (i > 0) {
			if (buf[bi][++len] == '\n') {
				i--;
				len++;
			}
		}
		/* Skip NULLs between strings.  There should be just
		 * one but this is safer.  Also ensure we don't make
		 * infinite loop here with this BUFSIZ check. */
		while (!buf[bi][len] && len < bufsiz) {
			len++;
		}
		fputs(&buf[bi][len], stdout); /* Result   \o/ \o/  */
	}
	free(buf[0]);
	free(buf[1]);
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
		printf("usage: %s [[-][+]n] [file]\n\n"
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
	if (!fp) {
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
