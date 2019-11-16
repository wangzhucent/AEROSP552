all: command_fan

command_fan: command.o da.o
	g++ -o command_fan command.o da.o

command.o: command.c
	g++ -c command.c

da.o: da.c
	g++ -c da.c
