/*
 * A subroutine version of the macro ferror
 */
#define	USE_STDIO_MACROS
#include <stdio.h>

#undef ferror

int
(ferror)(FILE *fp)
{
	return ferror(fp);
}
