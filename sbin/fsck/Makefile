#
# Copyright (c) 1980 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

OBJS		= main.o
#OBJS		= cross.o

CFLAGS		+= -Wall -Os

# The program itself
#
PROG		= fsck

# Sources
#
SRCS		= dir.c inode.c  pass1.c pass1b.c pass2.c \
		  pass3.c pass4.c pass5.c setup.c utilities.c \
		  main.c cross.c

# Objects
#
OBJS		+= dir.o inode.o pass1.o pass1b.o pass2.o \
		  pass3.o pass4.o pass5.o setup.o utilities.o

# Header files
#
HDRS		= fsck.h

all:		${PROG}

${PROG}:	${OBJS} ${HDRS}
		${CC} ${CFLAGS} ${LDFLAGS} -o fsck.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S fsck.elf > fsck.dis
		${SIZE} fsck.elf
		${ELF2AOUT} fsck.elf $@ && rm fsck.elf

install:	all
		${INSTALL} ${PROG} ${DESTDIR}/sbin/${PROG}

clean:
		rm -f *.o *~ *.elf *.dis a.out core ${PROG}

lint:
		lint ${INCPATH} ${SRCS}

depend:
		cc -M ${INCPATH} ${SRCS} | \
		awk ' { if ($$1 != prev) { print rec; rec = $$0; prev = $$1; } \
			else { if (length(rec $$2) > 78) { print rec; rec = $$0; } \
			       else rec = rec " " $$2 } } \
		      END { print rec } ' > makedep
		echo '/^# DO NOT DELETE THIS LINE/+2,$$d' >eddep
		echo '$$r makedep' >>eddep
		echo 'w' >>eddep
		cp Makefile Makefile.bak
		ed - Makefile < eddep
		rm eddep makedep
		echo '# DEPENDENCIES MUST END AT END OF FILE' >> Makefile
		echo '# IF YOU PUT STUFF HERE IT WILL GO AWAY' >> Makefile
		echo '# see make depend above' >> Makefile

# DO NOT DELETE THIS LINE -- make depend uses it
