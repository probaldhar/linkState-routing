/**
 * Implements all operation for client.
 *
 * @author : Probal chandra dhar
 * @Filename : server.c
 * @Date : 04/17/17
 * @course : COP5990
 * @Project # : 5
 * @Usage: ./node <routerLabel> <portNum> <totalNumRouters> <discoverFile> [-dynamic]
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#include	<sys/wait.h>
#include 	<netdb.h>
/* According to POSIX.1-2001, POSIX.1-2008 */
#include <sys/select.h>
/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include	"node.h"

// global adjacency matrix
int **adjMat;

int main(int argc,  char *argv[] ) {

	// For return values for socket creation, send & receive message
	int nodeSd, sendRet, recvlen, dynamic = 0, i, k, retval, numfd, receivedNeighborCounter;
	// sockaddr_in to send the message
	struct sockaddr_in ownAddress, serveraddress, recvAddress, neighbors[NUM_NEIGHBOR]; // neighbors to make the connections
	// length of sockaddr_in
	socklen_t addrlen = sizeof(serveraddress);;
	// get the host information
	struct hostent *hostptr;
	// store the hostname
	char hostname[128];
	// For storing the argument
    char *portNum, *rounterLabel, *totalNumRouters, *discoverFile, FileNameWithPath[BUFSIZE];

	// Link state packets for each record in the file
	allLSP allLSP, recvLSP;

	// select params
	fd_set readfds;
	struct timeval tv;

	// Usage
	if ( argc < 5 ){
		printf("Usage: node <routerLabel> <portNum> <totalNumRouters> <discoverFile> [-dynamic]\n");
		exit(1);
	}

    // setting memory for the argument strings separately
	rounterLabel = (char * )malloc(sizeof(char)*BUFSIZE);
	portNum = (char * )malloc(sizeof(char)*BUFSIZE);
	totalNumRouters = (char * )malloc(sizeof(char)*BUFSIZE);
	discoverFile = (char * )malloc(sizeof(char)*BUFSIZE);

	// check if dynamic
	if ( argc == 6 && !strcmp(argv[5], "-dynamic") ) {
		dynamic = 1; // setting the flag to 1
	} 

	/****** storing the arguments *****/
	rounterLabel = argv[1]; // router label
	portNum = argv[2]; // own port
	totalNumRouters = argv[3]; // Total number of routers
	discoverFile = argv[4]; // File for neighbor discovery

	//converting totalNumRouters - string to int
	int rowCol = atoi(totalNumRouters);
	// adjacency matrix

	// dynamic memory allocation for adjacency matrix
	adjMat = (int **)malloc(rowCol * sizeof(int *));
	for ( i = 0; i < rowCol; i++ )
		 adjMat[i] = (int *)malloc(rowCol * sizeof(int));

	// assign -1 to each of the record of adjMat
	for ( i = 0; i < rowCol; i++ ) {
		for ( k = 0; k < rowCol; k++ ) {

			if ( i == k ) {
				adjMat[i][k] = 0;
			} else {
				adjMat[i][k] = inf;
			}
		}
	}

printf("matrix check\n");
	printArray(rowCol, adjMat);

	// combining the filename with path
	if ( snprintf(FileNameWithPath, sizeof(FileNameWithPath), "%s%s", "nodes-pc/", discoverFile) < 0 ) {
		printf("somwthing went wrong with snprintf:");
		exit(1);
	}	

	// printing the arguments
	printf("Testing: %s %s %s %s\n", rounterLabel, portNum, totalNumRouters, discoverFile);


	// creating socket
	if ( (nodeSd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("socket creation failed");
		return -1;
	}

	// DO NOT DELETE THESE
	// // Get information about client:
	// if ( gethostname(hostname, sizeof(hostname) ) ){
	// 	printf("Error\n");
	// }

	// get network host entry
    // hostptr = gethostbyname(hostname);
    hostptr = gethostbyname("127.0.0.1"); // THIS SHOULD BE REPLACED BY THE PREVIOUS LINE

	// setting memory to recvAddress 
	memset( &recvAddress, 0, sizeof(recvAddress) );

	// own address
	memset( &ownAddress, 0, sizeof(ownAddress) );
	ownAddress.sin_family = AF_INET;
	ownAddress.sin_port = htons(atoi(portNum));//PORT NO
	ownAddress.sin_addr.s_addr = htonl(INADDR_ANY);//ADDRESS

	memcpy((void *)&ownAddress.sin_addr, (void *)hostptr->h_addr, hostptr->h_length); 

	// DO NOT DELETE IT
	// printing the hostname, IP & port
	// printf("name: %s\n", hostptr->h_name);
    printf("addr: [%s]\n", inet_ntoa(ownAddress.sin_addr));
    printf("port: %d\n",ntohs(ownAddress.sin_port));
	
	printf("router Starting service\n");

	// binding to a specific port
	if ( bind(nodeSd, (struct sockaddr *) &ownAddress, sizeof(ownAddress) ) < 0 ){
		perror("Bind Fails:");
		exit(1);
	}

	// File operation
	FILE *fp = fopen(FileNameWithPath, "r");
	// delimeter
	const char s[2] = ", ";
    char *token;

	// neighbor counter
	int neighborCounter = 0;

	// Check if the file is open or not
	if(fp == NULL)
    {
        perror("cannot open File:");
		exit(1);
    }

	// Tokenize a line from the file
	char line[32];
	while(fgets(line, sizeof(line), fp) != NULL)
	{

		// loop counter
		i = 0;

		/********* LSP create for each record in the file *********/

		// copying source router's label to allLSP
		// strcpy(allLSP.singleLSP[neighborCounter].source, rounterLabel);
		// hop counter
		allLSP.singleLSP[neighborCounter].hop = atoi(totalNumRouters) - 1;
		// sequence number, initially 1
		allLSP.singleLSP[neighborCounter].seqNum = 1;

		token = strtok(line, s);

		while (token!= NULL)
		{
			// routerLabel
			if ( i % 4 == 0 ) { // rounterLabel
				strcpy(allLSP.singleLSP[neighborCounter].label, token);
			} else if ( i % 4 == 1 ) { // IP_address/hostname
				strcpy(allLSP.singleLSP[neighborCounter].nodeIP, token);
			} else if ( i % 4 == 2 ) { // portNumber
				allLSP.singleLSP[neighborCounter].nodePort = atoi(token);
			} else {
				allLSP.singleLSP[neighborCounter].cost = atoi(token);
			}

			token = strtok (NULL, s);
			i++; // loop counter
		}

		neighborCounter++; // adding the neighbor counter

	}


	// initial sleep
	printf("initial 5 seconds delay so that every router can up & running.\n");
	sleep(5);

	// assign the number of neighbor in the LSP packet
	allLSP.numberOfNeighbor = neighborCounter;

	// assigning hop count to packet
	allLSP.hopCount = atoi(totalNumRouters) - 1;

	strcpy(allLSP.source, rounterLabel);

	// packet from FILE
	int j;
	for ( j = 0; j < neighborCounter; j++ ) {

		// initial matrix build
		adjMatrixChange( adjMat, allLSP.source, allLSP.singleLSP[j].label, allLSP.singleLSP[j].cost );

		// printing the values of LSPs
		printf("%s %d %d %s %s %d %d\n", allLSP.source, allLSP.singleLSP[j].hop, allLSP.singleLSP[j].seqNum, allLSP.singleLSP[j].label, allLSP.singleLSP[j].nodeIP, allLSP.singleLSP[j].nodePort, allLSP.singleLSP[j].cost);

		// sockaddr_in create for each record in the file
		memset( &neighbors[j], 0, sizeof(neighbors[j]) );
		neighbors[j].sin_family = AF_INET;
		neighbors[j].sin_port = htons(allLSP.singleLSP[j].nodePort);//PORT NO
		neighbors[j].sin_addr.s_addr = inet_addr(allLSP.singleLSP[j].nodeIP);//ADDRESS

		// DO NOT DELETE IT
		// memcpy((void *)&neighbors.sin_addr, (void *)hostptr->h_addr, hostptr->h_length); 

		// size of sockaddr_in
		addrlen = sizeof(neighbors[i]);

		// send "allLSP" to every record of the neighbors
		sendRet = sendto( nodeSd, &allLSP, sizeof(allLSP), 0, (struct sockaddr*)&neighbors[j],addrlen);

		if ( sendRet < 0 ) {
			perror("something went wrong while sending:");
			exit(1);
		}

	}

	// printing adjacency matrix so far
	printArray(rowCol, adjMat);

	/***** LOOP START *****/ // for receive & send

	while (1) {

		// select system call
		// add our descriptors to the set
		FD_ZERO(&readfds);
	    FD_SET(nodeSd, &readfds);

	    // the highest-numbered file descriptor
	    numfd = nodeSd + 1;

	    /* Wait up to five seconds. */
	    tv.tv_sec = 5;
	    tv.tv_usec = 0;

	    // checking return value of select
	    retval = select(numfd, &readfds, NULL, NULL, &tv);
	    /* Don't rely on the value of tv now! */

	    if (retval == -1)
	    	perror("select()");
	    else if (retval) {
	    	printf("Data is available now.\n");

	        // receive & send
	        // size of sockaddr_in
			addrlen = sizeof(recvAddress);

			// should wait for any receive message and send if hop > 0

			// receive "recvLSP" from other nodes
			recvlen = recvfrom( nodeSd, &recvLSP, sizeof(recvLSP), 0, (struct sockaddr*)&recvAddress, &addrlen) ;
		
			// bytes received
			// printf("%d", recvlen);

			// LSP counter in the packet received
			receivedNeighborCounter = recvLSP.numberOfNeighbor;

			// printing what received
			for ( j = 0; j < receivedNeighborCounter; j++ ) {
				printf("[%d] %s %d %d %s %s %d %d\n", recvLSP.numberOfNeighbor, recvLSP.source, recvLSP.singleLSP[j].hop, recvLSP.singleLSP[j].seqNum, recvLSP.singleLSP[j].label, recvLSP.singleLSP[j].nodeIP, recvLSP.singleLSP[j].nodePort, recvLSP.singleLSP[j].cost);

				// reform matrix
				adjMatrixChange( adjMat, recvLSP.source, recvLSP.singleLSP[j].label, recvLSP.singleLSP[j].cost );
			}

			// printing adjacency matrix so far
			printArray(rowCol, adjMat);


			// check the hop count & send

			// check the hop count & change the hop count(s) if necessary
			// in packet there can be more than one LSP. LSP contains the hop count
			// not the allLSP - should I add a hop in the allLSP ????????
			if ( recvLSP.hopCount > 0 ) { 

				printf("Hop Count: %d\n", recvLSP.hopCount);

				// reducing the hop count for a packet
				recvLSP.hopCount--;

				// send the packet to other neighbors
				for ( i = 0; i < neighborCounter; i++ ) {

					// check if the same source from where I got the packet
					if ( !strcmp (inet_ntoa(recvAddress.sin_addr), inet_ntoa(neighbors[i].sin_addr) ) && (ntohs(neighbors[i].sin_port) == ntohs(recvAddress.sin_port)) ) {
						// dont't send - because that's where you got the packet
					} else {
						// send the packet to other neighbors

						// size of sockaddr_in
						addrlen = sizeof(recvAddress);

						// send tha packet
						sendRet = sendto( nodeSd, &recvLSP, sizeof(recvLSP), 0, (struct sockaddr*)&neighbors[i],addrlen);

						// error checking
						if ( sendRet < 0 ) {
							perror("something went wrong while sending:");
							exit(1);
						}
					}
				}

			} // end if

	    } else {
	    	printf("No data within five seconds.\n");
	    	// break the while loop
	    	break;
	    }

	} // end of while(1)
	/***** LOOP END *****/ // for receive & send

	/********* start after receive *********

		1. check the hop count
		2. check where it's coming from
		3. send the packet if hop > 0 to others router
		4. wait for any other packets - wait for a certain time

	********** end after receive *********/

	// Final adjacency matrix
	printArray(rowCol, adjMat);

	// int routerName = rounterLabel[0] % 65;

	djikstra(adjMat, rounterLabel);

	fclose(fp);
	return 0;
}















