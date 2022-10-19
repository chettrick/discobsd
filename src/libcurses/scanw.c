/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */

/*
 * scanw and friends
 *
 */
#include <string.h>
#include "curses.ext"

/*
 *	This routine implements a scanf on the standard screen.
 */
int
scanw(fmt, args)
        char	*fmt;
        va_list	args; /* XXX */
{
	return _sscans(stdscr, fmt, args); /* XXX */
}
/*
 *	This routine implements a scanf on the given window.
 */
int
wscanw(win, fmt, args)
        WINDOW	*win;
        char	*fmt;
        va_list	args; /* XXX */
{
	return _sscans(win, fmt, args); /* XXX */
}

/*
 *	This routine actually executes the scanf from the window.
 *
 *	This is really a modified version of "sscanf".  As such,
 * it assumes that sscanf interfaces with the other scanf functions
 * in a certain way.  If this is not how your system works, you
 * will have to modify this routine to use the interface that your
 * "sscanf" uses.
 */
int
_sscans(win, fmt, args)
        WINDOW	*win;
        char	*fmt;
        va_list	args; /* XXX */
{
	char	buf[100];
	FILE	junk;

	junk._flag = _IOREAD|_IOSTRG;
	junk._base = junk._ptr = buf;
	if (wgetstr(win, buf) == ERR)
		return ERR;
	junk._cnt = strlen(buf);
	return _doscan(&junk, fmt, args); /* XXX */
}
