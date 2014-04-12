
SHELL = /bin/sh

# Name of the generated binary
FILE_OUT = tiny-ci

BIN = bin
SRC = src

CC = g++
CCFLAGS = -Wall -std=c++11

BOOST_INCLUDE_DIR=/usr/local/include/boost
BOOST_LIB_DIR=/usr/local/lib

all: $(BIN)/tiny-ci size
check: tester

$(BIN)/tiny-ci: $(BIN)/main.o $(BIN)/Test.o $(BIN)/MakeTest.o $(BIN)/Result.o $(BIN)/gitTools.o
	$(CC) -I $(BOOST_INCLUDE_DIR) -L $(BOOST_LIB_DIR) -Wl,-rpath,$(BOOST_LIB_DIR) $(CCFLAGS) $(BIN)/main.o $(BIN)/Test.o $(BIN)/MakeTest.o $(BIN)/Result.o $(BIN)/gitTools.o -lboost_regex -o $(BIN)/$(FILE_OUT)

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

tester: all tester.o testbindir
	$(CC) testbin/tester.o $(BIN)/MakeTest.o $(BIN)/gitTools.o $(BIN)/Result.o -o testbin/tester
	./testbin/tester

tester.o: testbindir tests/tester.c
	$(CC) $(CCFLAGS) -c tests/tester.c -o testbin/tester.o

testbindir:
	mkdir -p testbin

size:
	@echo
	ls -lah $(BIN)/$(FILE_OUT) | cut -d' ' -f5
