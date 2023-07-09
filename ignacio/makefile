CC = g++

# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
CFLAGS = -Wall -g
STD_V = -std=c++17

main: clean main.o matrix.o zvector.o trie.o
	$(CC) $(CFLAGS) -o main main.o matrix.o zvector.o trie.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp $(STD_V)

matrix.o: matrix/matrix.h
	$(CC) $(CFLAGS) -c matrix/matrix.cpp

zvector.o: zvector/zvector.h
	$(CC) $(CFLAGS) -c zvector/zvector.cpp

trie.o: trie/trie.h
	$(CC) $(CFLAGS) -c trie/trie.cpp

clean:
	$(RM) main main.o matrix.o zvector.o trie.o

clean_o:
	$(RM) main.o matrix.o zvector.o trie.o