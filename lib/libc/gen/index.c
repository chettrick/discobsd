/*
 * Return the ptr in sp at which the character c appears;
 * NULL if not found
 */

#define	NULL	0

char *
index(sp, c)
	register const char *sp;
	register int c;
{
	do {
		if (*sp == c)
			return((char *)sp);
	} while (*sp++);
	return(NULL);
}
