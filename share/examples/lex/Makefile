# Lex examples from the source tree:
#	awk, lex manpage example, cpp, and ccom.

LEX=	lex
LFLAGS=	-v

# These files require at least 96Kbytes of User RAM.
SRCS=	awk.lx.l example.l scanner.l

# These files require at least 120Kbytes of User RAM.
#SRCS=	scan.l

all:
	-@for src in ${SRCS} ; do \
		echo "Generating from: $$src" ; \
		${LEX} ${LFLAGS} $$src ; \
	done

clean:
	rm -f a.out [Ee]rrs *.core y.tab.[ch] lex.yy.c
