CC=gcc
CFLAGS=-I. -I./include
BUILD_DIR=./build
OUTPUT_DIR=./output
SRC_DIR=./src
TEST_DIR=./tests

$(shell mkdir -p $(BUILD_DIR))
$(shell mkdir -p $(OUTPUT_DIR)/logs)

clean:
	rm -rf $(BUILD_DIR)/*
	rm -f compiler

clean_output:
	rm -f $(OUTPUT_DIR)/logs/*.log


compiler: $(BUILD_DIR)/lex.yy.o $(BUILD_DIR)/parser.tab.o $(BUILD_DIR)/tab.o $(BUILD_DIR)/util.o $(BUILD_DIR)/codeGen.o $(BUILD_DIR)/main.o $(BUILD_DIR)/reg.o
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

$(BUILD_DIR)/reg.o: $(SRC_DIR)/reg.c $(wildcard $(BUILD_DIR)/*.h) $(wildcard ./include/*.h)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/lex.yy.c: $(SRC_DIR)/lexer.l $(BUILD_DIR)/parser.tab.h
	flex -o $@ $<

$(BUILD_DIR)/parser.tab.c $(BUILD_DIR)/parser.tab.h: $(SRC_DIR)/parser.y
	bison -d -v -t -Wcounterexamples $< -o $(BUILD_DIR)/parser.tab.c


test: compiler
	@if [ -z "$(file)" ]; then \
		echo "Error: No test file specified. Usage: make test file=<file_path>"; \
		exit 1; \
	fi
	@echo "===== Running Single Test =====\n"
	@echo "Input file: $(file)"
	@log_file_name=$$(basename "$(file)").log; \
	log_file_path=$(OUTPUT_DIR)/logs/$$log_file_name; \
	echo "Attempting to save log to: $$log_file_path\n"; \
	./compiler "$(file)" > "$$log_file_path"; \
	if [ $$? -eq 0 ]; then \
		echo "Test output for $(file) saved to $$log_file_path\n"; \
	else \
		echo "Error running compiler for $(file). Check stderr for messages. Log might be empty or incomplete: $$log_file_path"; \
	fi
	@echo "===== Single Test End ====="


test_all: compiler
	@if [ -z "$(dir)" ]; then \
		echo "Error: No directory specified. Usage: make test_all dir=<directory_path>"; \
		exit 1; \
	fi
	@if [ ! -d "$(dir)" ]; then \
		echo "Error: Directory $(dir) not found."; \
		exit 1; \
	fi
	@echo "===== Running All Tests in Directory =====\n"
	@echo "Target directory: $(dir)"
	@echo "Output directory for logs: $(OUTPUT_DIR)/logs \n"
	@for test_file in $(wildcard $(dir)/*); do \
		if [ -f "$$test_file" ]; then \
			echo "======================================"; \
			echo "Processing file: $$test_file \n"; \
			log_file_name=$$(basename "$$test_file").log; \
			log_file_path=$(OUTPUT_DIR)/logs/$$log_file_name; \
			echo "Attempting to save log to: $$log_file_path \n"; \
			./compiler "$$test_file" > "$$log_file_path"; \
			if [ $$? -eq 0 ]; then \
				echo "Test output for $$test_file saved to $$log_file_path"; \
			else \
				echo "Error running compiler for $$test_file. Check stderr for messages. Log might be empty ou incompleto: $$log_file_path \n"; \
			fi; \
		fi; \
	done
	@echo "======================================"
	@echo "===== All Tests in Directory End ====="