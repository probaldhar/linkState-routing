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
    
    // array of LSP
    LSP singleLSP[NUM_NEIGHBOR];

}allLSP;

#endif