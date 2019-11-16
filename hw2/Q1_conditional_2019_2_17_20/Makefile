all: test

test: test.o Tree.o
	g++ -o test test.o Tree.o

test.o: test.cpp
	g++ -c test.cpp

Tree.o: Tree.cpp
	g++ -c Tree.cpp