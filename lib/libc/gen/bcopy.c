/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

/*
 * bcopy -- vax movc3 instruction
 */
void
bcopy(src0, dst0, length)
	register const void *src0;
	register void *dst0;
	register unsigned int length;
{
	const char *src = src0;
	char *dst = dst0;

	if (length && src != dst) {
		if (dst < src) {
			do
				*dst++ = *src++;
			while (--length);
		} else {		/* copy backwards */
			src += length;
			dst += length;
			do
				*--dst = *--src;
			while (--length);
		}
	}
	return;
}
