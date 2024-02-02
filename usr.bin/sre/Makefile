CFLAGS         += -Os
OBJS            = tag.o cmd.o file.o main.o misc.o init.o render.o setup.o srch.o

all:            sre

sre:            $(OBJS)
		${CC} ${CFLAGS} ${LDFLAGS} ${OBJS} -o $@

clean:
		rm -f sre *.o *~

install:        all
		${INSTALL} sre ${DESTDIR}/bin/
