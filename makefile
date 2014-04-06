
SHELL = /bin/sh

# Name of the generated binary
FILE_OUT = tiny-ci

BIN = bin
SRC = src

CC = g++
CCFLAGS = -Wall

all: $(BIN)/tiny-ci
check: all

$(BIN)/tiny-ci: $(BIN)/main.o $(BIN)/Test.o $(BIN)/MakeTest.o $(BIN)/Result.o
	$(CC) $(CCFLAGS) $(BIN)/main.o $(BIN)/Test.o $(BIN)/MakeTest.o $(BIN)/Result.o -o $(BIN)/$(FILE_OUT)

$(BIN)/main.o: $(SRC)/main.cpp $(BIN)
	$(CC) $(CCFLAGS) -c $(SRC)/main.cpp -o $(BIN)/main.o

$(BIN)/Test.o: $(SRC)/Test.cpp $(BIN)
	$(CC) $(CCFLAGS) -c $(SRC)/Test.cpp -o $(BIN)/Test.o

$(BIN)/MakeTest.o: $(SRC)/MakeTest.cpp $(BIN)
	$(CC) $(CCFLAGS) -c $(SRC)/MakeTest.cpp -o $(BIN)/MakeTest.o

$(BIN)/Result.o: $(SRC)/Result.cpp $(BIN)
	$(CC) $(CCFLAGS) -c $(SRC)/Result.cpp -o $(BIN)/Result.o

$(BIN): 
	mkdir -p $(BIN)
