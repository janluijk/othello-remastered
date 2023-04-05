# eenvoudige makefile
MAKEFLAGS += --silent

all: main.o othello.o stapel.o
	g++ -Wall -g -o ../build/Othello othello.o main.o stapel.o
	valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all ../build/Othello 
	#../build/Othello
stapel.o: stapel.cc
	g++ -Wall -g -c stapel.cc

othello.o: othello.cc 
	g++ -Wall -g -c othello.cc

main.o: main.cc 
	g++ -Wall -g -c main.cc

clean:
	rm *.o