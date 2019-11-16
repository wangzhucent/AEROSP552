all: test

test: test.o automaton.o
	g++ -std=c++11 -o test test.o automaton.o

test.o: test.cpp
	g++ -std=c++11 -c test.cpp

automaton.o: automaton.cpp
	g++ -std=c++11 -c automaton.cpp
