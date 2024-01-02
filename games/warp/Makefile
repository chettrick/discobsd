TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS		+= -Os -Wall

OBJS            = bang.o init.o intrp.o move.o \
                  object.o play.o score.o \
                  sig.o term.o them.o us.o \
                  util.o version.o warp.o weapon.o
LIBS            = -ltermcap -lm
MAN		= warp.0
MANSRC		= warp.man
PRIVATE         = warp.doc smap.0 smap.1 smap.2 smap.3 smap.4 smap.5 smap.6 smap.7
PRIVLIB         = ${DESTDIR}/usr/games/lib/warp

all:            warp $(MAN) $(PRIVATE)

warp:           ${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o warp.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S warp.elf > warp.dis
		${SIZE} warp.elf
		${ELF2AOUT} warp.elf $@ && rm warp.elf

smap.0:         smp.0 sm
		./sm <smp.0 >smap.0
smap.1:         smp.1 sm
		./sm <smp.1 >smap.1
smap.2:         smp.2 sm
		./sm <smp.2 >smap.2
smap.3:         smp.3 sm
		./sm <smp.3 >smap.3
smap.4:         smp.4 sm
		./sm <smp.4 >smap.4
smap.5:         smp.5 sm
		./sm <smp.5 >smap.5
smap.6:         smp.6 sm
		./sm <smp.6 >smap.6
smap.7:         smp.7 sm
		./sm <smp.7 >smap.7
sm:             sm.c
		${HOST_CC} -O -Wall sm.c -o sm

${MAN}:         ${MANSRC}
		${MANROFF} ${MANSRC} > $@

clean:
		rm -f *.o *.elf ${MAN} warp *.elf *.dis tags *~ smap.* sm

install:        warp $(PRIVATE)
		install warp ${DESTDIR}/usr/games/
		cp ${MAN} ${DESTDIR}/usr/share/man/cat6/
		mkdir -p $(PRIVLIB)
		cp $(PRIVATE) $(PRIVLIB)
		cp /dev/null $(PRIVLIB)/save.blurfl
		[ -f $(PRIVLIB)/warp.news ] || cp warp.news $(PRIVLIB)
