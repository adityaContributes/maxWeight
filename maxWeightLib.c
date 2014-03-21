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
void InitQueues(My402List *Q)
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
			//	dbg(dbgMsg);
			}
		}
	}
}

//It is Bernouli function with probability of returning 1 (packet arriving) as load 
int packetArrives(double load)
{
	double t1 = rand() % 10;
	double t2 = 10*load;
	if ( t1 <= t2 )	
	{
		return(1);
	}
	else
	{
		return(-1);
	}
}
//It wil return the destination port equaly likely
//**Works only for 2 port. It will return either 0 or 1 with equal probabilities
int destPort(double p0)
{
	//printf("pirobability:%g\n",p0);
	if (rand() % 10 < p0*10)
		return(0);
	else
		return(1);
}

//Will fill the 2D array of weights "w" with the queue lengths
void getWeight(int *w,My402List *Q)
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

void maxWeight(int *w,int *toDequeue)
{
	//int i,j;
	//The algorithm for max weight matching graph should come here. 
	int tmp = 0;
	int r = 0;
	if ( *w + *(w+3) > *(w+1) + *(w+2) )
	{
		*toDequeue = 1;		//toDequeue[0][0] = 1;
		*(toDequeue+1) = 0;	//toDequeue[0][1] = 0;
		*(toDequeue+2) = 0;	//toDequeue[1][0] = 0;
		*(toDequeue+3) = 1;	//toDequeue[1][1] = 1;
	}
	else if ( *w + *(w+3) < *(w+1) + *(w+2) )
	{
		*toDequeue = 0;		//toDequeue[0][0] = 0;
		*(toDequeue+1) = 1;	//toDequeue[0][1] = 1;
		*(toDequeue+2) = 1;	//toDequeue[1][0] = 1;
		*(toDequeue+3) = 0;	//toDequeue[1][1] = 0;

	}
	else if ( *w + *(w+3) == *(w+1) + *(w+2) )
	{
		r = rand();
		tmp = r % 2;
		//printf("r:%d tmp:%d\n",r,tmp);
		if ( 0 == tmp )
		{
			printf("##\n");
			*toDequeue = 1;		//toDequeue[0][0] = 1;
			*(toDequeue+1) = 0;	//toDequeue[0][1] = 0;
			*(toDequeue+2) = 0;	//toDequeue[1][0] = 0;
			*(toDequeue+3) = 1;	//toDequeue[1][1] = 1;
			type1++;
		}
		else
		{
			printf("$$\n");
			*toDequeue = 0;		//toDequeue[0][0] = 0;
			*(toDequeue+1) = 1;	//toDequeue[0][1] = 1;
			*(toDequeue+2) = 1;	//toDequeue[1][0] = 1;
			*(toDequeue+3) = 0;	//toDequeue[1][1] = 0;
			type2++;
		}
	}

}
//Parse the config file and populate the load and probabilities of destinations
void parseConfig()
{
	FILE *fp;
	char buf[1024];
	char *ptr,*bufPtr;
	if (NULL == (fp = fopen("config","r")))
	{
		fprintf(stderr,"Could not open config file\n");
		exit(1);
	}
	//Get simTime
	if (NULL != fgets(buf, 100,fp))
	{
		bufPtr = buf;
		while (*bufPtr != ':')
		{
			(bufPtr)++;
		}
		bufPtr++;
		ptr = bufPtr;
		while (*ptr != '\n')
			ptr++;
		*ptr = '\0';
		simTime = atof(bufPtr);
		sprintf(dbgMsg,"simTime:%d(int)\n",simTime);
		dbg(dbgMsg);
	}
	//Get load[0]
	if (NULL != fgets(buf, 100,fp))
	{
		bufPtr = buf;
		while (*bufPtr != ':')
		{
			(bufPtr)++;
		}
		bufPtr++;
		ptr = bufPtr;
		while (*ptr != '\n')
			ptr++;
		*ptr = '\0';
		load[0] = atof(bufPtr);
		sprintf(dbgMsg,"Load0:%f (double)\n",load[0]);
		dbg(dbgMsg);
	}
	//Get probability to destination port
	if (NULL != fgets(buf, 100,fp))
	{
		bufPtr = buf;
		while (*bufPtr != ':')
		{
			(bufPtr)++;
		}
		bufPtr++;
		ptr = bufPtr;
		while (*ptr != ' ')
		{	
			ptr++;
		}
		*ptr = '\0';
		p[0][0] = atof(bufPtr);
		sprintf(dbgMsg,"P00:%g (double)\n",p[0][0]);
		dbg(dbgMsg);
		
		ptr++;
		bufPtr = ptr;
		while (*bufPtr != ':')
		{
			(bufPtr)++;
		}
		bufPtr++;
		ptr = bufPtr;
		while (*ptr != '\n')
		{
			ptr++;
		}
		*ptr = '\0';
		p[0][1] = atof(bufPtr);
		sprintf(dbgMsg,"P01:%g (double)\n",p[0][1]);
		dbg(dbgMsg);
	}
	//Get load[1]
	if (NULL != fgets(buf, 100,fp))
	{
		bufPtr = buf;
		while (*bufPtr != ':')
		{
			(bufPtr)++;
		}
		bufPtr++;
		ptr = bufPtr;
		while (*ptr != '\n')
			ptr++;
		*ptr = '\0';
		load[1] = atof(bufPtr);
		sprintf(dbgMsg,"Load0:%f (double)\n",load[1]);
		dbg(dbgMsg);
	}
	//Get probability to destination port
	if (NULL != fgets(buf, 100,fp))
	{
		bufPtr = buf;
		while (*bufPtr != ':')
		{
			(bufPtr)++;
		}
		bufPtr++;
		ptr = bufPtr;
		while (*ptr != ' ')
		{	
			ptr++;
		}
		*ptr = '\0';
		p[1][0] = atof(bufPtr);
		sprintf(dbgMsg,"P10:%g (double)\n",p[1][0]);
		dbg(dbgMsg);
		
		ptr++;
		bufPtr = ptr;
		while (*bufPtr != ':')
		{
			(bufPtr)++;
		}
		bufPtr++;
		ptr = bufPtr;
		while (*ptr != '\n')
		{
			ptr++;
		}
		*ptr = '\0';
		p[1][1] = atof(bufPtr);
		sprintf(dbgMsg,"P11:%g (double)\n",p[1][1]);
		dbg(dbgMsg);
	}
}
