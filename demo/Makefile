# Makefile
#
# part of the DEMO compiler
#
# H. Toussaint (hto@info.fundp.ac.be), 14/06/05


all:
	bison -dty demo.y
	flex demo.l
	gcc -ggdb -c *.c
	gcc -ggdb -o demo *.o

clean:
	-rm *.o *~ y.tab.c y.tab.h y.output lex.yy.c 


distclean: clean
	-rm demo
