CC = g++

# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
CFLAGS = -Wall -g
STD_V = -std=c++23

main: clean main.o file_reader.o
	$(CC) $(CFLAGS) -o main main.o file_reader.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp $(STD_V)

file_reader.o: file_reader/file_reader.h
	$(CC) $(CFLAGS) -c file_reader/file_reader.cpp

clean:
	$(RM) main main.o file_reader.o

clean_o:
	$(RM) main.o file_reader.o