CC = g++
CFLAGS = -c -std=c++11 -g -Wall
OUTPUT = jane

all: jane
jane : main.o Ipros.o
	       	$(CC) $(LFLAGS) Ipros.o main.o -lboost_system -o $(OUTPUT)
main.o : main.cpp Ipros.hpp Ipros.o
		$(CC) $(CFLAGS) main.cpp

Ipros.o: Ipros.cpp Ipros.hpp util.hpp
		$(CC) $(CFLAGS) Ipros.cpp
clean:
		rm -rf main.o Ipros.o util.o

