CC=gcc
CFLAGS=-Wall -Wextra -pedantic

out: main.c lib.o
	$(CC) $(CFLAGS) main.c lib.o -o main

lib.o: Node.o Stack.o String.o LinkedList.o
	$(CC) $(CFLAGS) -r Node.o Stack.o String.o LinkedList.o -o lib.o

Stack.o: Stack.c Stack.h LinkedList.h Node.h
	$(CC) $(CFLAGS) -c Stack.c -o Stack.o

String.o: DataTypes/String.c DataTypes/String.h
	$(CC) $(CFLAGS) -c DataTypes/String.c -o String.o

LinkedList.o: LinkedList.c LinkedList.h Node.h
	$(CC) $(CFLAGS) -c LinkedList.c -o LinkedList.o

Node.o: Node.c Node.h
	$(CC) $(CFLAGS) -c Node.c -o Node.o

clean:
	del *.o
	del *.exe

run:
	make
	./main
	make clean