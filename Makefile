CC=gcc
CFLAGS=-I. -I./parser
BUILD_DIR=./build

$(shell mkdir -p $(BUILD_DIR))

clean:
	rm -rf $(BUILD_DIR)/*
	rm -f compilador

run:
	(bison -d -v -t -Wcounterexamples ./parser/parser.y -o $(BUILD_DIR)/parser.tab.c && \
	flex -o $(BUILD_DIR)/lex.yy.c lexer.l && \
	$(CC) $(CFLAGS) -c $(BUILD_DIR)/lex.yy.c -o $(BUILD_DIR)/lex.yy.o && \
	$(CC) $(CFLAGS) -c $(BUILD_DIR)/parser.tab.c -o $(BUILD_DIR)/parser.tab.o && \
	$(CC) $(CFLAGS) -c ./parser/util.c -o $(BUILD_DIR)/util.o && \
	$(CC) $(CFLAGS) -o compilador $(BUILD_DIR)/lex.yy.o $(BUILD_DIR)/parser.tab.o $(BUILD_DIR)/util.o -lfl)