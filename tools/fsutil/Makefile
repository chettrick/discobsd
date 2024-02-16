TOPSRC!=cd ../..; pwd

-include ../Makefile.inc

CFLAGS		= -O -Wall
OBJS		= fsutil.o superblock.o block.c inode.o create.o check.o \
                  file.o mount.o manifest.o
PROG		= fsutil

# For Mac OS X
FUSE_PATH!=if [ x"${_HOST_OSNAME}" = x"Darwin" ] ; then \
		echo "/usr/local/lib/pkgconfig" ; \
	else \
		echo "" ; \
	fi

# Fuse
MOUNT_CFLAGS!=	PKG_CONFIG_PATH=${FUSE_PATH} pkg-config fuse --cflags
LIBS!=		PKG_CONFIG_PATH=${FUSE_PATH} pkg-config fuse --libs

all:		$(PROG)

install:	all
		${INSTALL} ${PROG} ${TOOLBINDIR}/${PROG}

clean:
		rm -f *~ *.o ${PROG} ${PROG}.dSYM

$(PROG):	$(OBJS)
		$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

root.bin:       $(PROG)
		./$(PROG) -n16384 -s2048 $@
		./$(PROG) -v $@
		./$(PROG) -c $@

swap.bin:
		dd bs=1k count=2048 < /dev/zero > $@

mount.o: mount.c bsdfs.h
		$(CC) $(CFLAGS) $(MOUNT_CFLAGS) -c -o $@ mount.c

block.o: block.c bsdfs.h
check.o: check.c bsdfs.h
create.o: create.c bsdfs.h
file.o: file.c bsdfs.h
fsutil.o: fsutil.c bsdfs.h manifest.h
inode.o: inode.c bsdfs.h
manifest.o: manifest.c bsdfs.h manifest.h
superblock.o: superblock.c bsdfs.h
