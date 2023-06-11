all: config.c cpy.c egrep.c expr.c gram.c grammar.c parser.c

%.c: %.y
	yacc $<
	mv y.tab.c $@

clean:
	rm -f *.c yacc.acts yacc.tmp y.output y.tab.[ch]
