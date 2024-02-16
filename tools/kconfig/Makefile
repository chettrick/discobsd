TOPSRC!=cd ../..; pwd

-include ../Makefile.inc

CFLAGS		= -O -Wall

OBJS            = config.o main.o lang.o mkioconf.o mkmakefile.o mkswapconf.o
PROG		= kconfig
MAN		= kconfig.0
MANSRC		= kconfig.8

all:		${PROG} ${MAN}

install:	all
		${INSTALL} ${PROG} ${TOOLBINDIR}/${PROG}

${MAN}:		${MANSRC}
		${MANROFF} ${MANSRC} > $@

clean:
		rm -f *~ *.o ${PROG} ${MAN} ${PROG}.dSYM y.tab.[ch] lang.c config.c

$(PROG):	$(OBJS)
		$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

main.o: main.c y.tab.h config.h
mkheaders.o: mkheaders.c config.h y.tab.h
mkioconf.o: mkioconf.c y.tab.h config.h
mkmakefile.o: mkmakefile.c y.tab.h config.h
mkswapconf.o: mkswapconf.c config.h
