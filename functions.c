/**
 * functions for node.
 *
 * @author : Probal chandra dhar
 * @Filename : functions.c
 * @Date : 04/20/17
 * @course : COP5990
 * @Project # : 5
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "node.h"


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
void adjMatrixChange( int **adjMat, char *sourceRouter, char *labelRouter, int cost ) {

	// value of source & destination routerLabel
	int source, dest;
	
	// integer value of source routerLabel
	source = sourceRouter[0] % 65;
	// printf("%d ", source);
	
	// integer value of dest routerLabel
	dest = labelRouter[0] % 65;
	// printf("%d\n", dest);
	
	// changing the matrix value
	adjMat[source][dest] = cost;
	// printf("%d ", adjMat[source][dest]);
	adjMat[dest][source] = cost;
	// printf("%d\n", adjMat[dest][source]);

}

/**
 * Printing the current adjacency matrix
 *
 * @param   n - number of row & column in the adjacency matrix
 * @param   array - the adjacency matrix
 *
 */
void printArray ( int n, int **array ) {
	int i, j;
	
	for ( i = 0; i < n; i++ ) {
		for ( j = 0; j < n; j++ ) {
			if (array[i][j] != inf )
				printf("%5d ", array[i][j]);
			else 
				printf("  inf ");
		}
		printf("\n");		
	}
}

/**
 * calculating minimum between two values
 *
 * @param   x - first value
 * @param   y - second value
 *
 * @return	y if x>y and x otherwise
 *
 */
int min ( int x, int y )
{
	return ( x>y ? y:x );
}


/**
 * calculating shortest path for a router to go to others router 
 * and priting the forwaring table using djikstra's algorithm.
 *
 * @param   adjMat 			- adjacency matrix
 * @param   rounterLabel 	- source router label
 * @param	totalNumRouters	- total number of router
 *
 */
void djikstra ( int **adjMat, char *rounterLabel, int totalNumRouters ) {

	// cost matrix
	struct node costMat[totalNumRouters+1][totalNumRouters];
	// visited nodes
	int visited[totalNumRouters];
	// Router rootNode
	int vIndex; 
	vIndex = rounterLabel[0] % 65;
	int minIndex,minVal;
	
	int i,j;
	int markedVal;

	// initializing visited matrix to 0
	for ( i = 0; i < totalNumRouters; i++ )
		visited[i] = 0;
	
	
	// fill the first row of the cost matrix as infinity
	for ( i = 0; i < totalNumRouters; i++ )
		if(i==vIndex)
			costMat[0][i].cost=0; 
		else 
			costMat[0][i].cost=inf;
			
	markedVal=0;

	for ( i = 1; i < totalNumRouters + 1; i++ )
	{
		visited[vIndex]=1; // mark the first node as visited
		minVal=inf;
		/**scan through the matrix for neighbouring nodes */
		for( j=0; j < totalNumRouters; j++ )
		{
			if(visited[j]==1)
			{	
				costMat[i][j].cost=costMat[i-1][j].cost;
				costMat[i][j].prev=costMat[i-1][j].prev;
				//	if(costMat[i][j].prev){}
				//	else costMat[i][j].prev=vIndex;
				//markedVal=costMat[i][j];
				//vIndex++;
				//printf("The value is %d \n",costMat[i][j]);
				continue;
				   // if the node is already visiteifd, just move on to the next node
			} else {
				if(min(markedVal+adjMat[vIndex][j],costMat[i-1][j].cost)==costMat[i-1][j].cost)
				{
					costMat[i][j].cost=costMat[i-1][j].cost;
					costMat[i][j].prev=costMat[i-1][j].prev;
				} else {
						costMat[i][j].prev=vIndex;
						costMat[i][j].cost=	min(markedVal+adjMat[vIndex][j],costMat[i-1][j].cost);
				}

				if(costMat[i][j].cost<minVal)
				{
					minVal=costMat[i][j].cost;
					minIndex=j;
				}
			}
			
			//vIndex=minIndex;
			//markedVal=minVal;
			//printf("marked val: %d\n",markedVal);
		}
		vIndex=minIndex;
		markedVal=minVal;
	//	printf("MArked value %d\n",markedVal);
	}
	
	costMat[totalNumRouters][rounterLabel[0] % 65].prev = -1;
	// printing the cost Matrix
	
	for ( i = 1; i < totalNumRouters+1; i++ )
	{
		for(j=0;j<totalNumRouters;j++)
		{
			if (costMat[i][j].cost!=5000 && j != vIndex ) 
				printf("%d %c  ",costMat[i][j].cost,costMat[i][j].prev+65);
			else 
				printf("inf");
		}
		
		printf("\n");
	}
		
		
	/** the path to be searched for a specific router */
	
	printf("\nForwarding table for: %s\n\n", rounterLabel);

	int search;

	for ( search = 0; search < totalNumRouters; search ++ ) {

		i=totalNumRouters;
		j=search;

		// printf("search: %d\n", search);
		// printf("check: i=%d j=%d %d\n", i, j, costMat[i][j].prev);

	
		while ( costMat[i][j].prev != -1 && costMat[i][costMat[i][j].prev].cost!=0)
		{
			j=costMat[i][j].prev;
		}
		
		printf("The router hop to %c is %c\n", 65+search, j+65);

	}
		
	
}











