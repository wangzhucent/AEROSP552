all: test

test: tree_main.o Tree.o
	g++ -o test tree_main.o Tree.o

tree_main.o: tree_main.cpp
	g++ -c tree_main.cpp

Tree.o: Tree.cpp
	g++ -c Tree.cpp
