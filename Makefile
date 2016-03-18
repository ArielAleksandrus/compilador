CC=g++
CFLAGS=-I. -std=c++11
DEBUGFLAGS=$(CFLAGS) -g
SOURCE=FileReader.cpp Token.cpp
MAIN=main.cpp $(SOURCE)
TEST=test/test_$(FILE) $(SOURCE)
BIN=bin/a.out

all: clean $(MAIN)
	$(CC) $(MAIN) $(CFLAGS) -o $(BIN)

debug: clean $(MAIN)
	$(CC) $(MAIN) $(DEBUGFLAGS) -o $(BIN)
	
test: clean $(TEST)
	$(CC) $(TEST) $(CFLAGS) -o $(BIN)

test_debug: clean $(TEST)
	$(CC) $(TEST) $(DEBUGFLAGS) -o $(BIN)

clean:
	rm -f *.o
	rm -f $(BIN)