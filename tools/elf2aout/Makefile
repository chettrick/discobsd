TOPSRC!=cd ../..; pwd

-include ../Makefile.inc

CFLAGS		= -O -Wall
OBJS		= elf2aout.o
PROG		= elf2aout

# For Mac OS X
#LIBS		= -largp

all:		$(PROG)

install:	all
		install ${PROG} ${TOOLBINDIR}/${PROG}

clean:
		rm -f *~ *.o *.lst *.dis $(PROG)

$(PROG):	$(OBJS)
		$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

$(PROG).exe:	$(OBJS)
		$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)
