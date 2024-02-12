TOPSRC!=cd ../..; pwd

-include ../Makefile.inc

OBJS    = mkrd.o
PROG    = mkrd
CFLAGS  = -Wall -O

all: ${PROG}

${PROG}: ${OBJS}
	$(CC) -o $@ $(OBJS) $(LIBS) $(LDFLAGS)

install: all
	${INSTALL} ${PROG} ${TOOLBINDIR}/${PROG}

clean:
	rm -f ${PROG} ${OBJS}
