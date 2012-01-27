# Makefile
#
# part of the DEMO compiler
#
# H. Toussaint (hto@info.fundp.ac.be), 14/06/05


all: distclean
	bison -dty *.y
	flex *.l
	gcc -ggdb -DYYDEBUG=1 -c *.c
	gcc -ggdb -o lsd12 *.o

clean:
	-rm *.o *~ y.tab.c y.tab.h y.output lex.yy.c 

test: all
	./lsd12 < tests/*

distclean: clean
	-rm lsd12
