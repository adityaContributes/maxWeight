#ifdef DEBUG
#       define dbg(x) printf("DEBUG: %s",x);
#else
#       define dbg(x) {} //NO DEBUG
#endif
#include <stdio.h>
#include <stdlib.h>
#include "my402list.h"
#include "maxWeight.h"


//Initialise all queues
void InitQueues(My402List *Q, int numPort)
{	
	int i,j;
	for (i = 0; i < numPort; i++)
	{
		for (j = 0; j < numPort; j++)
		{
			if (!My402ListInit( &Q[i * numPort + j] ) )
			{
				fprintf(stderr,"Error initialisind queue for input port %d and dest port %d\n",i,j);
				exit(1);
			}
			else
			{
				sprintf(dbgMsg,"Initilised Queue: Input Port:%d Destination Port:%d\n",i,j);
				dbg(dbgMsg);
			}
		}
	}
}

//It is Bernouli function with probability of returning 1 (packet arriving) as load 
int packetArrives(double load)
{
	if (rand() % 10 < 10*load)	
		return(1);
	else
		return(-1);
}
//It wil return the destination port equaly likely
//**Works only for 2 port. It will return either 0 or 1 with equal probabilities
int destPort(int numPort)
{
	if (rand() % 10 < 4.5)
		return(0);
	else
		return(1);
}

//Will fill the 2D array of weights "w" with the queue lengths
void getWeight(int *w,My402List *Q,int numPort)
{
	int i,j;
	for (i = 0; i < numPort; i++)
	{
		for (j =0; j < numPort; j++)
		{
			*(w + (i*numPort) + j) = My402ListLength(&Q[i * numPort + j]);	
		}
	}
}

//Will Calculate the max weight graph and fill the toDequeue array.

void maxWeight(int *w,int *toDequeue,int numPort)
{
	//int i,j;
	//The algorithm for max weight matching graph should come here. 
	if ( *w + *(w+3) > *(w+1) + *(w+2) )
	{
		*toDequeue = 1;		//toDequeue[0][0] = 1;
		*(toDequeue+1) = 0;	//toDequeue[0][1] = 0;
		*(toDequeue+2) = 0;	//toDequeue[1][0] = 0;
		*(toDequeue+3) = 1;	//toDequeue[1][1] = 1;
	}
	else
	{
		*toDequeue = 0;		//toDequeue[0][0] = 0;
		*(toDequeue+1) = 1;	//toDequeue[0][1] = 1;
		*(toDequeue+2) = 1;	//toDequeue[1][0] = 1;
		*(toDequeue+3) = 0;	//toDequeue[1][1] = 0;

	}
}
