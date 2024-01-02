TOPSRC!=cd ../../../; pwd
include ${TOPSRC}/share/mk/sys.mk

CC      = ${HOST_CC}
OBJS    = smux.o
BIN     = smux
CFLAGS  = -O -Wall

$(BIN): $(OBJS)
	$(CC) -o $(BIN) $(OBJS)

clean:
	rm -f smux.o smux
