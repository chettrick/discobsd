TOPSRC!=cd ../..; pwd
include $(TOPSRC)/target.mk

SRCS            = reboot.c
OBJS            = reboot.o
MAN             = reboot.0
MANSRC          = reboot.8

CFLAGS          += -Wall -Werror

all:            reboot ${MAN}

reboot:         ${OBJS}
		${CC} ${LDFLAGS} -o reboot.elf ${OBJS} -lc
		${OBJDUMP} -S reboot.elf > reboot.dis
		${SIZE} reboot.elf
		${ELF2AOUT} reboot.elf $@
		ln -f $@ halt
		ln -f $@ poweroff

${MAN}:         ${MANSRC}
		${MANROFF} ${MANSRC} > ${MAN}

clean:
		rm -f *.o *~ *.elf *.dis ${MAN} reboot halt poweroff tags

depend:         ${SRCS}
		mkdep ${CFLAGS} ${SRCS}

install:        all
		cp ${MAN} ${DESTDIR}/usr/share/man/cat8/
		@-rm -f ${DESTDIR}/usr/share/man/cat8/fastboot.0 ${DESTDIR}/usr/share/man/cat8/halt.0
		ln ${DESTDIR}/usr/share/man/cat8/reboot.0 ${DESTDIR}/usr/share/man/cat8/fastboot.0
		ln ${DESTDIR}/usr/share/man/cat8/reboot.0 ${DESTDIR}/usr/share/man/cat8/halt.0
		install reboot ${DESTDIR}/sbin/reboot
		@-rm -f ${DESTDIR}/sbin/fastboot ${DESTDIR}/sbin/halt ${DESTDIR}/sbin/poweroff
		ln -f ${DESTDIR}/sbin/reboot ${DESTDIR}/sbin/halt
		ln -f ${DESTDIR}/sbin/reboot ${DESTDIR}/sbin/fastboot
		ln -f ${DESTDIR}/sbin/reboot ${DESTDIR}/sbin/poweroff
		ln -f ${DESTDIR}/sbin/reboot ${DESTDIR}/sbin/bootloader

lint:           ${SRCS}
		lint -hax ${SRCS}

tags:           ${SRCS}
		ctags ${SRCS}

# DO NOT DELETE THIS LINE -- mkdep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
