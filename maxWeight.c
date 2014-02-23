#ifdef DEBUG
#	define dbg(x) printf("DEBUG: %s",x);
#else
#	define dbg(x) {} //NO DEBUG
#endif
#include <stdlib.h>
#include "my402list.h"
#include "maxWeight.h"
#include <stdio.h>
#include <error.h>
#include <string.h>
#define numPort 2
#define simTime 100
int main (int argc, char * argv[])
{
	int timeSlot = 0;
	int packet_c = 0;
	int i,j;
	int dest,pcktArrived;	//destination port of packet arrived
	double load;//The load on the switch
	int weight[numPort][numPort];
	int toDequeue[numPort][numPort]; //an element will be 1,if packet from port corresponding to index of this element is to be served
	My402ListElem *f; //Used for dequeue
	packet *srvdPckt; //Used for dequeue
	memset(&toDequeue,0,sizeof(toDequeue));
	midPoint = RAND_MAX / 2;
	load = 1;
	My402List Q[numPort * numPort];
	memset(&Q,0,sizeof(Q));
	
	InitQueues(&Q,numPort);		//Initialise input queues
	srand (time(NULL));
	
	for (timeSlot = 0; timeSlot < simTime; timeSlot++)
	{
	//Generate packet
	for (i = 0; i < numPort; i++)	//For all input ports
	{
		dest = -1;
		pcktArrived = -1;
		if ((pcktArrived = packetArrives(load)) == 1)	//Packet arrived
		{
			packet_c++;
			dest = destPort(numPort);	//Get the destination port
			packet * pkt = (packet *) malloc(sizeof(packet));
			pkt->timeEnter = timeSlot;
			pkt->timeLeft = -1;
			pkt->dest = dest;
			pkt->pkt_num = packet_c;
			if (!(My402ListAppend(&Q[i * numPort + dest],(void *)pkt)))
			{
				fprintf(stderr,"Error Appending packet%d to Queue%d%d\n",packet_c,i,dest);
				exit(1);
			}
	
			printf("TimeSlot:%d\tp%d\ti/p Port:%d\to/p Port:%d\n",timeSlot,packet_c,i,dest);
		}
	}
	getWeight(weight,&Q,numPort);
	sprintf(dbgMsg,"W00:%d\tW01:%d\tW10:%d\tW11:%d\n",weight[0][0],weight[0][1],weight[1][0],weight[1][1]);
	dbg(dbgMsg);
	maxWeight(weight,toDequeue,numPort);	//Do the biparted graph matching
	sprintf(dbgMsg,"D00:%d\tD01:%d\tD10:%d\tD11:%d\n",toDequeue[0][0],toDequeue[0][1],toDequeue[1][0],toDequeue[1][1]);
	dbg(dbgMsg);

	//Service a packet based on maxWeight. Use the toDequeue array.
	//This assumes that toDequeue array willhave atleast one queue marked as 1 per input port
	for (i = 0; i < numPort; i++)
	{
		j = 0;
		while (toDequeue[i][j] != 1)
		{
			j++;
		}
		if (weight[i][j] > 0) //i.e. if Q[i][j] is not empty
		{
			f = My402ListFirst(&Q[i * numPort + j]);
			if (f != NULL)
			{
				srvdPckt = (packet *) f->obj;
				My402ListUnlink(&Q[i * numPort + j],f);
				srvdPckt->timeLeft = timeSlot;
			}
			else
			{
				fprintf(stderr,"LogiCAL flaw. Even if weight is greater than zero the Q[%d][%d] is empty\n",i,j);
				exit(1);
			}
			printf("TimeSlot:%d\tp%d\tfrom port:%d\tto port:%d\tserviced\n",timeSlot,srvdPckt->pkt_num,i,j);
		}
	}
	}
	return(0);
}
