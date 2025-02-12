clean:
	rm -f lex.yy.* parser.output parser.tab.* compilador

run:
	bison -d -v parser.y
	flex lexer.l
	gcc -c lex.yy.c
	g++ -o compilador lex.yy.o parser.tab.c lex.yy.c -lfl
	