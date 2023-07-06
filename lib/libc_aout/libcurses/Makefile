TOPSRC!=cd ../../..; pwd
include $(TOPSRC)/target.mk

LIBDIR=		${TOPSRC}/lib/libcurses

COPTS!=if [ x"${MACHINE_ARCH}" = x"mips" ] ; then \
		echo "-Wa,-x -B${TOPSRC}/lib/libc_aout/" ; \
	else \
		echo "" ; \
	fi

CFLAGS		= -Os ${DEFS} -Wall ${COPTS}

OBJS		= addch.o addstr.o box.o clear.o clrtobot.o clrtoeol.o cr_put.o \
                  cr_tty.o curses.o delch.o deleteln.o delwin.o endwin.o erase.o \
                  fullname.o getch.o getstr.o idlok.o id_subwins.o initscr.o insch.o \
                  insertln.o longname.o move.o mvprintw.o mvscanw.o mvwin.o newwin.o \
                  overlay.o overwrite.o printw.o putchar.o refresh.o scanw.o scroll.o \
                  toucholap.o standout.o touchwin.o tstp.o unctrl.o

all:            ../libcurses.a

%.o:		${LIBDIR}/%.c
		${CC} ${DEFS} ${CFLAGS} -c $< -o $@

../libcurses.a:	${OBJS}
		../ar_aout rc $@ ${OBJS}
		../ranlib_aout $@

install:	../libcurses.a
		${INSTALLDIR} ${DESTDIR}/usr/lib
		${INSTALL} ../libcurses.a ${DESTDIR}/usr/lib/libcurses.a
		../ranlib_aout -t ${DESTDIR}/usr/lib/libcurses.a

clean:
		rm -f *~ *.o a.out *.a ../libcurses.a


addch.o:	${LIBDIR}/addch.c
addstr.o:	${LIBDIR}/addstr.c
box.o:		${LIBDIR}/box.c
clear.o:	${LIBDIR}/clear.c
clrtobot.o:	${LIBDIR}/clrtobot.c
clrtoeol.o:	${LIBDIR}/clrtoeol.c
cr_put.o:	${LIBDIR}/cr_put.c
cr_tty.o:	${LIBDIR}/cr_tty.c
curses.o:	${LIBDIR}/curses.c
delch.o:	${LIBDIR}/delch.c
deleteln.o:	${LIBDIR}/deleteln.c
delwin.o:	${LIBDIR}/delwin.c
endwin.o:	${LIBDIR}/endwin.c
erase.o:	${LIBDIR}/erase.c
fullname.o:	${LIBDIR}/fullname.c
getch.o:	${LIBDIR}/getch.c
getstr.o:	${LIBDIR}/getstr.c
idlok.o:	${LIBDIR}/idlok.c
id_subwins.o:	${LIBDIR}/id_subwins.c
initscr.o:	${LIBDIR}/initscr.c
insch.o:	${LIBDIR}/insch.c
insertln.o:	${LIBDIR}/insertln.c
longname.o:	${LIBDIR}/longname.c
move.o:		${LIBDIR}/move.c
mvprintw.o:	${LIBDIR}/mvprintw.c
mvscanw.o:	${LIBDIR}/mvscanw.c
mvwin.o:	${LIBDIR}/mvwin.c
newwin.o:	${LIBDIR}/newwin.c
overlay.o:	${LIBDIR}/overlay.c
overwrite.o:	${LIBDIR}/overwrite.c
printw.o:	${LIBDIR}/printw.c
putchar.o:	${LIBDIR}/putchar.c
refresh.o:	${LIBDIR}/refresh.c
scanw.o:	${LIBDIR}/scanw.c
scroll.o:	${LIBDIR}/scroll.c
toucholap.o:	${LIBDIR}/toucholap.c
standout.o:	${LIBDIR}/standout.c
touchwin.o:	${LIBDIR}/touchwin.c
tstp.o:		${LIBDIR}/tstp.c
unctrl.o:	${LIBDIR}/unctrl.c
