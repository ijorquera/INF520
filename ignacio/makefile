CC = g++

# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
CFLAGS = -Wall -g

main: main.o matrix.o zvector.o
	$(CC) $(CFLAGS) -o main main.o matrix.o zvector.o

main.o: main.cpp matrix.h zvector.h
	$(CC) $(CFLAGS) -c main.cpp

matrix.o: matrix.h

zvector.o: zvector.h

clean:
	$(RM) main main.o matrix.o zvector.o

clean_o:
	$(RM) main.o matrix.o zvector.o