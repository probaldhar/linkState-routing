all: node select

node: node.o
	gcc -o node node.c

node.o: node.h node.c
	gcc -g -c node.c -Wall

select: select.o
	gcc -o select select.c

select.o: select.c
	gcc -g -c select.c -Wall

clean:
	rm *.o node select