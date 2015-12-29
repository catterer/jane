CC = g++
CFLAGS = -c -std=c++11 -g -Wall
OUTPUT = jane

all: jane
jane : main.o 
	       	$(CC) $(LFLAGS) main.o -lboost_system -o $(OUTPUT)
main.o : main.cpp util.hpp
		$(CC) $(CFLAGS) main.cpp
clean:
		rm -rf main.o

