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
#include 	<unistd.h>
#include	"node.h"

int main(int argc,  char *argv[] ) {

	// For return values for socket creation, send & receive message
	int nodeSd, sendRet, recvlen, dynamic = 0, i;
	// sockaddr_in to send the message
	struct sockaddr_in ownAddress, serveraddress, recvAddress, neighbors[NUM_NEIGHBOR]; // neighbors to make the connections
	// length of sockaddr_in
	socklen_t addrlen = sizeof(serveraddress);;
	// get the host information
	struct hostent *hostptr;
	// store the hostname
	char hostname[128];
	// For storing the argument
    char *portNum, *rounterLabel, *totalNumRouters, *discoverFile, sendbuf[BUFSIZE], readbuf[BUFSIZE], FileNameWithPath[BUFSIZE];

	// Link state packets for each record in the file
	allLSP allLSP, recvLSP;

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

	// combining the filename with path
	if ( snprintf(FileNameWithPath, sizeof(FileNameWithPath), "%s%s", "nodes/", discoverFile) < 0 ) {
		printf("somwthing went wrong with snprintf:");
		exit(1);
	}	

	printf("Testing: %s %s %s %s\n", rounterLabel, portNum, totalNumRouters, discoverFile);


	// creating socket
	if ( (nodeSd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("socket creation failed");
		return -1;
	}

	// Get information about client:
	if ( gethostname(hostname, sizeof(hostname) ) ){
		printf("Error\n");
	}

	// get network host entry
    hostptr = gethostbyname(hostname);
	
	// server's address
	// memset( &serveraddress, 0, sizeof(serveraddress) );
	// serveraddress.sin_family = AF_INET;
	// serveraddress.sin_port = htons(atoi(hostPort));//PORT NO
	// serveraddress.sin_addr.s_addr = inet_addr(hostIP);//ADDRESS

	// setting memory to recvAddress 
	memset( &recvAddress, 0, sizeof(recvAddress) );

	// own address
	memset( &ownAddress, 0, sizeof(ownAddress) );
	ownAddress.sin_family = AF_INET;
	ownAddress.sin_port = htons(atoi(portNum));//PORT NO
	ownAddress.sin_addr.s_addr = htonl(INADDR_ANY);//ADDRESS

	memcpy((void *)&ownAddress.sin_addr, (void *)hostptr->h_addr, hostptr->h_length); 

	// printing the hostname, IP & port
	printf("name: %s\n", hostptr->h_name);
    printf("addr: [%s]\n", inet_ntoa(ownAddress.sin_addr));
    printf("port: %d\n",ntohs(ownAddress.sin_port));
	
	printf("client Starting service\n");

	// binding to a specific port
	if ( bind(nodeSd, (struct sockaddr *) &ownAddress, sizeof(ownAddress) ) < 0 ){
		perror("Bind Fails:");
		exit(1);
	}

	// copyting "new" to sendbuf string
	// strcpy(sendbuf, "new");
	
	// sendRet = sendto(nodeSd, sendbuf, strlen(sendbuf), 0, (struct sockaddr*)&serveraddress,addrlen);

	// if ( sendRet < 0 )
	// {
	// 	perror("Error while sending:");
	// 	exit(1);
	// }

	// recvlen = recvfrom( nodeSd, readbuf, BUFSIZE, 0, (struct sockaddr*)&serveraddress, &addrlen) ;
	// printf("received %d bytes\n", recvlen);
	// if (recvlen > 0) {

	// 	readbuf[recvlen] = 0;
	// 	// sscanf(readbuf, "This is packet %d",&indexofACK);
	// 	printf("received message: \"%s\"\n", &readbuf[0]);
	// } else {
	// 	perror("Error while receiving:");
	// 	exit(1);
	// }

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

		// hop counter
		allLSP.singleLSP[neighborCounter].hop = atoi(totalNumRouters) - 1;
		// sequence number, initially 1
		allLSP.singleLSP[neighborCounter].seqNum = 1;

		token = strtok(line, s);

		while (token!= NULL)
		{
			// routerLabel
			if ( i % 4 == 0 ) { // rounterLabel
				//printf ("0. %s ",token);
				strcpy(allLSP.singleLSP[neighborCounter].label, token);
			} else if ( i % 4 == 1 ) { // IP_address/hostname
				//printf ("1. %s ",token);
				strcpy(allLSP.singleLSP[neighborCounter].nodeIP, token);
			} else if ( i % 4 == 2 ) { // portNumber
				//printf ("2. %s ",token);
				allLSP.singleLSP[neighborCounter].nodePort = atoi(token);
			} else {
				//printf ("3. %s ",token); // cost
				allLSP.singleLSP[neighborCounter].nodePort = atoi(token);
			}

			// printf ("%s ",token);
			token = strtok (NULL, s);
			i++; // loop counter
		}

		neighborCounter++; // adding the neighbor counter

		// printf("\n");
	}

	int j;
	for ( j = 0; j < neighborCounter; j++ ) {
		printf("%d %d %s %s %d %d\n", allLSP.singleLSP[j].hop, allLSP.singleLSP[j].seqNum, allLSP.singleLSP[j].label, allLSP.singleLSP[j].nodeIP, allLSP.singleLSP[j].nodePort, allLSP.singleLSP[j].cost);

		// sockaddr_in create for each record in the file
		memset( &neighbors[j], 0, sizeof(neighbors[j]) );
		neighbors[j].sin_family = AF_INET;
		neighbors[j].sin_port = htons(allLSP.singleLSP[j].nodePort);//PORT NO
		neighbors[j].sin_addr.s_addr = inet_addr(allLSP.singleLSP[j].nodeIP);//ADDRESS

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

	// size of sockaddr_in
	addrlen = sizeof(recvAddress);

	// receive "allLSP" from other nodes
	recvlen = recvfrom( nodeSd, &recvLSP, sizeof(recvLSP), 0, (struct sockaddr*)&recvAddress, &addrlen) ;
	
	fclose(fp);
	return 0;
}