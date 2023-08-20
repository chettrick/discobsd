TOPSRC!=cd ../..; pwd

-include ../Makefile.inc

CFLAGS		= -O -Wall
OBJS		= ice2aout.o
PROG		= ice2aout

# For Mac OS X
#LIBS		= -largp

all:		$(PROG)

install:	all
		install ${PROG} ${TOOLBINDIR}/${PROG}

clean:
		rm -f *~ *.o *.lst *.dis $(PROG)

$(PROG):	$(OBJS)
		$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)
