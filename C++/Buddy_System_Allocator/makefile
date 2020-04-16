# makefile

all: memtest

Ackerman.o: Ackerman.cpp 
	g++ -std=c++11 -g -c Ackerman.cpp

BuddyAllocator.o : BuddyAllocator.cpp
	g++ -std=c++11 -g -c BuddyAllocator.cpp

Main.o : Main.cpp
	g++ -std=c++11 -g -c Main.cpp

memtest: Main.o Ackerman.o BuddyAllocator.o
	g++ -std=c++11 -o memtest Main.o Ackerman.o BuddyAllocator.o

clean:
	rm *.o
