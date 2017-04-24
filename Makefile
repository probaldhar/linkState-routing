all: node 
# all: node dij

node: functions.o node.o
	gcc -o node functions.o node.c

# dij: dij.o
# 	gcc -o dij djikstra.c

# dij.o: djikstra.c
# 	gcc -g -c djikstra.c -Wall

node.o: node.h node.c
	gcc -g -c node.c -Wall

functions.o: node.h functions.c
	gcc -g -c functions.c -Wall

clean:
	rm *.o node
	# rm *.o node dij