/*
 * Return the ptr in sp at which the character c last
 * appears; NULL if not found
 */

#define NULL 0

char *
rindex(sp, c)
	register const char *sp;
	register int c;
{
	register char *r;

	r = NULL;
	do {
		if (*sp == (char)c)
			r = (char *)sp;
	} while (*sp++);
	return(r);
}
