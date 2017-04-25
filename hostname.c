/**
 * Implements all operation for server.
 *
 * @author : Probal chandra dhar
 * @Filename : server.c
 * @Date : 04/17/17
 * @course : COP5990
 * @Project # : 5
 * @Usage: ./server <serever's port no>
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

#define BUFSIZE 512

int main(int argc,  char *argv[] )

{
	int serverfd;
	struct sockaddr_in myaddr;
	struct hostent *hostptr;
    char hostname[128];

    if ( argc < 3 ){
		printf("Usage: server <portNum> <hostname/IP>\n");
		exit(1);
	}

	if ( (serverfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("socket creation failed");
		return -1;
	}

	strcpy(hostname, argv[2]);

	// Get information about destination host:
	// if ( gethostname(hostname, sizeof(hostname)) == -1 ){
	// 	printf("Error\n");
	// }

	// get network host entry
    hostptr = gethostbyname(hostname);

    if ( hostptr == NULL ){
    	perror("NULL:");
    	return 0;
    }


    memset((void *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = inet_addr(hostname);
	myaddr.sin_port = htons(atoi(argv[1]));

	memcpy((void *)&myaddr.sin_addr, (void *)hostptr->h_addr, hostptr->h_length);

	printf("name: %s\n", hostptr->h_name);
    printf("addr: [%s]\n", inet_ntoa(myaddr.sin_addr));
   	printf("port: %d\n",ntohs(myaddr.sin_port));

   	return 0;

}