clean:
	rm -rf ./build/*
	rm -f compilador
run:
	bison -d -v -Wcounterexamples parser.y
	flex lexer.l
	gcc -c lex.yy.c
	gcc -c parser.tab.c
	gcc -o compilador lex.yy.o parser.tab.o -lfl
	mv lex.yy.* parser.output parser.tab.* ./build/