OBJS    = mkrd.o
BIN     = mkrd
CFLAGS  = -Wall -O

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS) $(LDFLAGS)

clean:
	@rm -f $(BIN) $(OBJS)
