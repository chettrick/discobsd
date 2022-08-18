/*
 * Compare strings:  s1>s2: >0  s1==s2: 0  s1<s2: <0
 */
#include <string.h>

int
strcmp(s1, s2)
	register const char *s1, *s2;
{

	while (*s1 == *s2++)
		if (*s1++=='\0')
			return(0);
	return(*s1 - *--s2);
}
