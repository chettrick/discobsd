/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

/*
 * Somewhat modified to key on strstr(" instead of error(", for use
 * as a general purpose string stripper.  Also replaces pointer with
 * a (long) pointer.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/stat.h>

#define	ungetchar(c)	ungetc(c, stdin)

/*
 * mkstr - create a string error message file by massaging C source
 *
 * Bill Joy UCB August 1977
 *
 * Modified March 1978 to hash old messages to be able to recompile
 * without addding messages to the message file (usually)
 *
 * Based on an earlier program conceived by Bill Joy and Chuck Haley
 *
 * Program to create a string error message file
 * from a group of C programs.  Arguments are the name
 * of the file where the strings are to be placed, the
 * prefix of the new files where the processed source text
 * is to be placed, and the files to be processed.
 *
 * The program looks for 'strstr("' in the source stream.
 * Whenever it finds this, the following characters from the '"'
 * to a '"' are replaced by 'seekpt' where seekpt is a
 * pointer into the error message file.
 * If the '(' is not immediately followed by a '"' no change occurs.
 *
 * The optional '-' causes strings to be added at the end of the
 * existing error message file for recompilation of single routines.
 */

FILE	*mesgread, *mesgwrite;
char	*progname;
char	usagestr[] = "usage: %s [ - ] mesgfile prefix file ...\n";
char	name[PATH_MAX], *np;

#define	NBUCKETS	511

struct	hash {
	long	hval;
	long hpt;
	struct	hash *hnext;
} *bucket[NBUCKETS];

static int
fgetNUL(char *obuf, int rmdr, FILE *file)
{
	register int c;
	register char *buf = obuf;

	while (--rmdr > 0 && (c = getc(file)) != 0 && c != EOF)
		*buf++ = c;
	*buf++ = 0;
	getc(file);
	if (feof(file) || ferror(file))
	    return 0;
	return 1;
}

static long
hashit(char *str, int really, unsigned int fakept)
{
	int i;
	register struct hash *hp;
	char buf[512];
	long hashval = 0;
	register char *cp;

	if (really)
		fflush(mesgwrite);
	for (cp = str; *cp;)
		hashval = (hashval << 1) + *cp++;
	i = hashval % NBUCKETS;
	if (i < 0)
		i += NBUCKETS;
	if (really != 0)
		for (hp = bucket[i]; hp != 0; hp = hp->hnext)
		if (hp->hval == hashval) {
			fseek(mesgread, (long) hp->hpt, 0);
			fgetNUL(buf, sizeof buf, mesgread);
#ifdef DEBUG
			fprintf(stderr, "Got (from %ld) %s\n", hp->hpt, buf);
#endif
			if (strcmp(buf, str) == 0)
				break;
		}
	if (!really || hp == 0) {
		hp = (struct hash *) calloc(1, sizeof *hp);
		hp->hnext = bucket[i];
		hp->hval = hashval;
		hp->hpt = really ? ftell(mesgwrite) : fakept;
		if (really) {
			fwrite(str, sizeof (char), strlen(str) + 1, mesgwrite);
			fwrite("\n", sizeof (char), 1, mesgwrite);
		}
		bucket[i] = hp;
	}
#ifdef DEBUG
	fprintf(stderr, "%s hashed to %ld at %ld\n", str, hp->hval, hp->hpt);
#endif
	return (hp->hpt);
}

static int
octdigit(int c)
{
	return (c >= '0' && c <= '7');
}

static void
copystr(void)
{
	register int c, ch;
	char buf[512];
	register char *cp = buf;

	for (;;) {
		c = getchar();
		if (c == EOF)
			break;
		switch (c) {

		case '"':
			*cp++ = 0;
			goto out;
		case '\\':
			c = getchar();
			switch (c) {

			case 'b':
				c = '\b';
				break;
			case 't':
				c = '\t';
				break;
			case 'r':
				c = '\r';
				break;
			case 'n':
				c = '\n';
				break;
			case '\n':
				continue;
			case 'f':
				c = '\f';
				break;
			case '0':
				c = 0;
				break;
			case '\\':
				break;
			default:
				if (!octdigit(c))
					break;
				c -= '0';
				ch = getchar();
				if (!octdigit(ch))
					break;
				c <<= 7, c += ch - '0';
				ch = getchar();
				if (!octdigit(ch))
					break;
				c <<= 3, c+= ch - '0', ch = -1;
				break;
			}
		}
		*cp++ = c;
	}
out:
	*cp = 0;
	printf("%ld", hashit(buf, 1, 0));
}

static int
match(char *ocp)
{
	register char *cp;
	register int c;

	for (cp = ocp + 1; *cp; cp++) {
		c = getchar();
		if (c != *cp) {
			while (ocp < cp)
				putchar(*ocp++);
			ungetchar(c);
			return (0);
		}
	}
	return (1);
}

static void
process(void)
{
	register int c;

	for (;;) {
		c = getchar();
		if (c == EOF)
			return;
		if (c != 's') {
			putchar(c);
			continue;
		}
		if (match("strstr(")) {
			printf("strstr(");
			c = getchar();
			if (c != '"')
				putchar(c);
			else
				copystr();
		}
	}
}

static void
inithash(void)
{
	char buf[512];
	int mesgpt = 0;

	rewind(mesgread);
	while (fgetNUL(buf, sizeof buf, mesgread) != 0) {
		hashit(buf, 0, mesgpt);
		mesgpt += strlen(buf) + 2;
	}
}

int
main(int argc, char *argv[])
{
	size_t n;
	char addon = 0;

	argc--, progname = *argv++;
	if (argc > 1 && argv[0][0] == '-')
		addon++, argc--, argv++;
	if (argc < 3)
		fprintf(stderr, usagestr, progname), exit(1);
	mesgwrite = fopen(argv[0], addon ? "a" : "w");
	if (mesgwrite == NULL)
		perror(argv[0]), exit(1);
	mesgread = fopen(argv[0], "r");
	if (mesgread == NULL)
		perror(argv[0]), exit(1);
	inithash();
	argc--, argv++;
	if ((n = strlcpy(name, argv[0], sizeof(name))) >= sizeof(name)) {
		fprintf(stderr, "%s too long\n", argv[0]);
		exit(1);
	}
	np = name + n;
	argc--, argv++;
	do {
		if (strlcpy(np, argv[0], sizeof(name)-n) >= sizeof(name)-n) {
			fprintf(stderr, "%s too long\n", argv[0]);
			exit(1);
		}
		if (freopen(name, "w", stdout) == NULL)
			perror(name), exit(1);
		if (freopen(argv[0], "r", stdin) == NULL)
			perror(argv[0]), exit(1);
		process();
		argc--, argv++;
	} while (argc > 0);
	exit(0);
}
