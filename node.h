/**
 * Defines all functions/structures for node.
 *
 * @author : Probal chandra dhar
 * @Filename : node.h
 * @Date : 04/17/17
 * @course : COP5990
 * @Project # : 5
 *
 */

#ifndef _NODE_H
#define _NODE_H

#define BUFF 64
#define BUFSIZE 512
#define NUM_NEIGHBOR 10

#define inf 5000

// Structure to LSP
typedef struct LSP
{	
	// hop counter
    int hop;
    // sequence number
    int seqNum;
    // router's label
    char label[BUFF];
    // connection information
    char nodeIP[BUFF];
    int nodePort;
    // cost information
    int cost;

}LSP;

// Structure to LSP packets
typedef struct allLSP
{	
    // keeping track of the router number
    int numberOfNeighbor;
    // hop count of packet
    int hopCount;
    // source router's label
    char source[BUFF];
    // array of LSP
    LSP singleLSP[NUM_NEIGHBOR];

}allLSP;


#define inf 5000

struct node
{
    int cost;
    int prev;
};


/**
 * calculating minimum between two values
 *
 * @param   x - first value
 * @param   y - second value
 *
 * @return  y if x>y and x otherwise
 *
 */
int min( int x, int y );

/**
 * changing the matrix value with respect to routerLabel
 * routerLabel must be one character, starting from A - must be capital
 * Formula used: routerLabel % 65
 *
 * @param   adjMat - adjacency matrix
 * @param   sourceRouter - label of source the router
 * @param   labelRouter - label of dest the router
 * @param   cost - cost of a path
 *
 */
void adjMatrixChange( int **adjMat, char *sourceRouter, char *labelRouter, int cost );

/**
 * Printing the current adjacency matrix
 *
 * @param   n - number of row & column in the adjacency matrix
 * @param   array - the adjacency matrix
 *
 */
void printArray(int n, int **array);

/**
 * calculating shortest path for a router to go to others router 
 * and priting the forwaring table using djikstra's algorithm.
 *
 * @param   adjMat - adjacency matrix
 * @param   rounterLabel - source router label
 *
 */
void djikstra(int **adjMat, char *rounterLabel, int totalNumRouters);

/**
 * flooding packets, receiving with system call select() to wait for a certain time
 *
 * @param   nodeSd          - socket descriptor
 * @param   neighbors[]     - array of nrighbor router connection
 * @param   rowCol          - number of router
 * @param   adjMat          - adjacency matrix
 * @param   neighborCounter - number of neighbor of a router
 *
 */
void floodReceiveWithSelect( int nodeSd, struct sockaddr_in neighbors[NUM_NEIGHBOR], int rowCol, int **adjMat, int neighborCounter );

#endif





