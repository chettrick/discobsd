/*
 * Returns the number of
 * non-NULL bytes in string argument.
 */
#include <string.h>

size_t
strlen(s)
	register const char *s;
{
	register int n;

	n = 0;
	while (*s++)
		n++;
	return(n);
}
