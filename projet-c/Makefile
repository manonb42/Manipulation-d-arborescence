.PHONY: build run clean deps

SRC_DIR = .
BUILD_DIR = .out

build: main.o arbre.o parse.o commands.o
	gcc -O1 -o main .out/main.o .out/arbre.o .out/parse.o .out/commands.o

run: build
	./main

clean:
	rm -f $(BUILD_DIR)/*.o main

deps:
	gcc -MM ./*.c

%.o: $(SRC_DIR)/%.c
	-mkdir -p $(BUILD_DIR)
	gcc -g -O1 -Wall -o $(BUILD_DIR)/$@ -c $<
