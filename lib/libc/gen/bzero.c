/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)bzero.c	1.1 (Berkeley) 1/19/87
 */

#include <string.h>

/*
 * bzero -- vax movc5 instruction
 */
void
bzero(void *b, size_t length)
{
	char *p;

	if (length) {
		p = b;
		do {
			*p++ = '\0';
		} while (--length);
	}
}
