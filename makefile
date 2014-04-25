
SHELL = /bin/sh

# Name of the generated binary
FILE_OUT = tiny-ci

BIN = bin
SRC = src

CC = g++
CCFLAGS = -Wall -std=c++11

BOOST_INCLUDE_DIR=/usr/local/include/boost
BOOST_LIB_DIR=/usr/local/lib

TINY_HOME = ~/.tiny-ci

all: $(BIN)/tiny-ci size
check: tester
install: all $(TINY_HOME)/processError.py

$(BIN)/tiny-ci: $(BIN)/main.o $(BIN)/Test.o $(BIN)/MakeTest.o $(BIN)/MavenTest.o $(BIN)/BashTest.o $(BIN)/Result.o $(BIN)/gitTools.o $(BIN)/database.o
	$(CC) -I $(BOOST_INCLUDE_DIR) -L $(BOOST_LIB_DIR) -Wl,-rpath,$(BOOST_LIB_DIR) $(CCFLAGS) $(BIN)/main.o $(BIN)/Test.o $(BIN)/MakeTest.o $(BIN)/MavenTest.o $(BIN)/BashTest.o $(BIN)/Result.o $(BIN)/gitTools.o $(BIN)/database.o -lboost_regex -lsqlite3 -o $(BIN)/$(FILE_OUT)

$(BIN)/main.o: $(SRC)/main.cpp
	$(CC) $(CCFLAGS) -c $(SRC)/main.cpp -o $(BIN)/main.o

$(BIN)/Test.o: $(SRC)/Test.cpp
	$(CC) $(CCFLAGS) -c $(SRC)/Test.cpp -o $(BIN)/Test.o

$(BIN)/MakeTest.o: $(SRC)/MakeTest.cpp
	$(CC) $(CCFLAGS) -c $(SRC)/MakeTest.cpp -o $(BIN)/MakeTest.o

$(BIN)/MavenTest.o: $(SRC)/MavenTest.cpp
	$(CC) $(CCFLAGS) -c $(SRC)/MavenTest.cpp -o $(BIN)/MavenTest.o

$(BIN)/BashTest.o: $(SRC)/BashTest.cpp
	$(CC) $(CCFLAGS) -c $(SRC)/BashTest.cpp -o $(BIN)/BashTest.o

$(BIN)/Result.o: $(SRC)/Result.cpp
	$(CC) $(CCFLAGS) -c $(SRC)/Result.cpp -o $(BIN)/Result.o

$(BIN)/gitTools.o: $(SRC)/gitTools.cpp
	$(CC) $(CCFLAGS) -c $(SRC)/gitTools.cpp -o $(BIN)/gitTools.o

$(BIN)/database.o: $(SRC)/database.cpp
	$(CC) $(CCFLAGS) -c $(SRC)/database.cpp -o $(BIN)/database.o

$(TINY_HOME)/processError.py: defaultProcessError.py
	cp defaultProcessError.py $(TINY_HOME)/processError.py

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
