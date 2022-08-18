/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

/*
 * bzero -- vax movc5 instruction
 */
void
bzero(b, length)
	register void *b;
	register unsigned int length;
{
	char *p;

	if (length) {
		p = b;
		do {
			*p++ = '\0';
		} while (--length);
	}
}
