# makefile

all: my_shell

shell.o: shell.cpp
	g++-8 -std=c++2a -c -g shell.cpp

main.o : main.cpp
	g++-8 -std=c++2a -c -g main.cpp

my_shell: shell.o
	g++-8 -std=c++2a -o my_shell shell.o

main: main.o
		g++-8 -std=c++2a -o main main.o

clean:
	rm *.o
