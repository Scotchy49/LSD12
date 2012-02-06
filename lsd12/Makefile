all:
	bison -dty *.y
	flex *.l
	gcc -ggdb -DYYDEBUG=1 -c *.c
	gcc -ggdb -o lsd12 *.o
profile:
	bison -dty *.y
	flex *.l
	gcc -pg -DYYDEBUG=1 *.c -o lsd12 -lprofiler 
clean:
	-rm *.o *~ y.tab.c y.tab.h y.output lex.yy.c 

test: all
	./lsd12 tests/syntax_long

distclean: clean
	-rm lsd12
