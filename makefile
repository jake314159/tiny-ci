
SHELL = /bin/sh

# Name of the generated binary
FILE_OUT = tiny-ci

BIN = bin
SRC = src

CC = g++
CCFLAGS = -Wall -std=c++11

all: $(BIN)/tiny-ci size
check: all

$(BIN)/tiny-ci: $(BIN)/main.o $(BIN)/Test.o $(BIN)/MakeTest.o $(BIN)/Result.o $(BIN)/gitTools.o
	$(CC) $(CCFLAGS) $(BIN)/main.o $(BIN)/Test.o $(BIN)/MakeTest.o $(BIN)/Result.o $(BIN)/gitTools.o -o $(BIN)/$(FILE_OUT)

$(BIN)/main.o: $(SRC)/main.cpp $(BIN)
	$(CC) $(CCFLAGS) -c $(SRC)/main.cpp -o $(BIN)/main.o

$(BIN)/Test.o: $(SRC)/Test.cpp $(BIN)
	$(CC) $(CCFLAGS) -c $(SRC)/Test.cpp -o $(BIN)/Test.o

$(BIN)/MakeTest.o: $(SRC)/MakeTest.cpp $(BIN)
	$(CC) $(CCFLAGS) -c $(SRC)/MakeTest.cpp -o $(BIN)/MakeTest.o

$(BIN)/Result.o: $(SRC)/Result.cpp $(BIN)
	$(CC) $(CCFLAGS) -c $(SRC)/Result.cpp -o $(BIN)/Result.o

$(BIN)/gitTools.o: $(SRC)/gitTools.cpp $(BIN)
	$(CC) $(CCFLAGS) -c $(SRC)/gitTools.cpp -o $(BIN)/gitTools.o

$(BIN): 
	mkdir -p $(BIN)

size:
	@echo
	ls -lah $(BIN)/$(FILE_OUT) | cut -d' ' -f5
