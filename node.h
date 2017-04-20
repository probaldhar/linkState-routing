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

void printArray(int n, int **array);

#endif





