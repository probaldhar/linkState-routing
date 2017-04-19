all: node

node: node.o
	gcc -o node node.c

node.o: node.h node.c
	gcc -g -c node.c -Wall

clean:
	rm *.o node