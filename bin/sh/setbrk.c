/*
 * UNIX shell
 *
 * Bell Telephone Laboratories
 */
#include "defs.h"

char 	*sbrk();

char *
setbrk(incr)
int	incr;
{
	register char *a = sbrk(incr);

	if (a != (char *)-1)		/* only if the break really moved */
		brkend = a + incr;
	return(a);
}
