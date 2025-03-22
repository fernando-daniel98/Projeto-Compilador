CC=gcc
CFLAGS=-I. -I./include
BUILD_DIR=./build
OUTPUT_DIR=./output
SRC_DIR=./src
TEST_DIR=./tests

$(shell mkdir -p $(BUILD_DIR))
$(shell mkdir -p $(OUTPUT_DIR)/ast)
$(shell mkdir -p $(OUTPUT_DIR)/logs)

clean:
	rm -rf $(BUILD_DIR)/*
	rm -f compiler

clean_output:
	rm -f $(OUTPUT_DIR)/logs/*.log
	rm -f $(OUTPUT_DIR)/ast/*.dot


compiler: $(BUILD_DIR)/lex.yy.o $(BUILD_DIR)/parser.tab.o $(BUILD_DIR)/tab.o $(BUILD_DIR)/util.o $(BUILD_DIR)/codeGen.o $(BUILD_DIR)/main.o
	$(CC) $(CFLAGS) -o $@ $^ -lfl

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/lex.yy.o: $(BUILD_DIR)/lex.yy.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/parser.tab.o: $(BUILD_DIR)/parser.tab.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/tab.o: $(SRC_DIR)/tab.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/util.o: $(SRC_DIR)/util.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/codeGen.o: $(SRC_DIR)/codeGen.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/lex.yy.c: $(SRC_DIR)/lexer.l $(BUILD_DIR)/parser.tab.h
	flex -o $@ $<

$(BUILD_DIR)/parser.tab.c $(BUILD_DIR)/parser.tab.h: $(SRC_DIR)/parser.y
	bison -d -v -t -Wcounterexamples $< -o $(BUILD_DIR)/parser.tab.c


test: compiler
	@if [ -z "$(file)" ]; then \
		echo "Error: No test file specified. Usage: make test file=<test_file>"; \
		exit 1; \
	fi
	./compiler $(TEST_DIR)/$(file) > $(OUTPUT_DIR)/logs/$(file)
	@echo "Test output saved to $(OUTPUT_DIR)/logs/$(file).txt"