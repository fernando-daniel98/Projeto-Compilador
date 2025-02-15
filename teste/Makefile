clean:
	rm -f lex.yy.* parser.output parser.tab.* parser.tab.h compilador
run:
	bison -d -v -Wcounterexamples parser.y
	flex lexer.l
	gcc -c lex.yy.c
	gcc -c parser.tab.c
	gcc -o compilador lex.yy.o parser.tab.o -lfl