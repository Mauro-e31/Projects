#makefile

all: shell

shell:	my_shell.o parser.o main.o
	g++ -std=c++11 -o shell main.o parser.o my_shell.o

my_shell.o: my_shell.cpp my_shell.h
	g++ -std=c++11 -g -c my_shell.cpp

parser.o: parser.cpp parser.h
	g++ -std=c++11 -g -c parser.cpp

main.o: main.cpp 
	g++ -std=c++11 -g -c main.cpp


clean:
	rm *.o

