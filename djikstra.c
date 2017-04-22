// a code to implement the djikstra algorithm

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define inf 5000

int min(int x,int y)
{
	return (x>y?y:x);
}
struct node
{
	int cost;
	int prev;
};

const int M=6;
const int N=6;

void djikstra(int adjMat[M][N], int rootNode);
int main()
{
	int adjMat[M][N]={ {0,2,1,inf,inf,inf},
						{2,0,inf,2,inf,inf},
						{1,inf,0,3,5,inf},
						{inf,2,3,0,4,inf},
						{inf,inf,5,4,0,6},
						{inf,inf,inf,inf,6,0}
						};
	

	djikstra(adjMat, 2);


	return 0;

}


void djikstra(int adjMat[M][N], int rootNode){

	struct node costMat[7][6];
	int visited[6]={0};
	int vIndex; // vIndex - argument
	vIndex = rootNode;
	int minIndex,minVal;
	
	int i,j;
	int markedVal;
	
	
	// fill the first row of the cost matrix as infinity
	for(i=0;i<6;i++)
		if(i==vIndex)
			costMat[0][i].cost=0; 
		else 
			costMat[0][i].cost=inf;
			
	markedVal=0;		
	for(i=1;i<7;i++)
	{
		visited[vIndex]=1; // mark the first node as visited
		minVal=inf;
		/**scan through the matrix for neighbouring nodes */
		for(j=0;j<6;j++)
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
	
	// printing the cost Matrix
	
	for ( i = 1; i < 7; i++ )
	{
		for(j=0;j<6;j++)
		{
			if(costMat[i][j].cost!=5000) printf("%d %c  ",costMat[i][j].cost,costMat[i][j].prev+65);
				else printf("inf");
		}
		
		printf("\n");
	}
		
		
	/** the path to be searched for a specific router */
	
	int search = 2;

	printf("Forwarding table for: %c\n", 65+rootNode);
	for ( search = 0; search < 6; search ++ ) {
		i=6;
		j=search;
		while(costMat[i][costMat[i][j].prev].cost!=0)
		{
			j=costMat[i][j].prev;
			// printf("The router hop to C is %c\n",j+65);
		}
		
		printf("The router hop to %c is %c\n", 65+search, j+65);
	}
		
	
}

