TOPSRC!=cd ../..; pwd

-include ../Makefile.inc

CFLAGS=		-O -Wall

# libbsd-dev package on Linux in overlay mode.
CFLAGS+=	${_LIBBSD_CFLAGS}
LIBS+=		${_LIBBSD_LIBS}

OBJS=		elf2aout.o
PROG=		elf2aout

# For Mac OS X
#LIBS+=		-largp

all:		$(PROG)

install:	all
		${INSTALL} ${PROG} ${TOOLBINDIR}/${PROG}

clean:
		rm -f *~ *.o ${PROG}

$(PROG):	$(OBJS)
		$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

$(PROG).exe:	$(OBJS)
		$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)
