CC=g++
CFLAGS=-I. -std=c++11
DEBUGFLAGS=$(CFLAGS) -g
SOURCE=*.cpp objects/*.cpp
MAIN=$(SOURCE)
TEST=test/test_$(FILE) $(SOURCE)
BIN=bin/a.out
MAKELOG=log/make.log

all: clean $(MAIN)
	$(CC) $(MAIN) $(CFLAGS) -o $(BIN)

debug: clean $(MAIN)
	$(CC) $(MAIN) $(DEBUGFLAGS) -o $(BIN)
	
log: clean $(MAIN)
	$(CC) $(MAIN) $(CFLAGS) -o $(BIN) > $(MAKELOG) 2>&1
test: clean $(TEST)
	$(CC) $(TEST) $(CFLAGS) -o $(BIN)

test_debug: clean $(TEST)
	$(CC) $(TEST) $(DEBUGFLAGS) -o $(BIN)

clean:
	rm -f *.o
	rm -f $(BIN)
	rm -f $(MAKELOG)