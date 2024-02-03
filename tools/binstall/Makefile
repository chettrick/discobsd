TOPSRC!=cd ../..; pwd

-include ../Makefile.inc

PROG=	xinstall
OBJS=	xinstall.o

# Use uninstalled copy of the install program.
INSTALL=./xinstall

all:	${PROG}

${PROG}: ${OBJS}
	${CC} ${LDFLAGS} -o $@ ${OBJS} ${LIBS}

install: all
	${INSTALL} ${PROG} ${TOOLBINDIR}/binstall

clean:
	rm -f *~ *.o ${PROG}

.PHONY: all install clean
