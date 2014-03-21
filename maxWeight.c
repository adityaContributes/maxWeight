#ifdef DEBUG
#	define dbg(x) printf("%s",x);
#else
#	define dbg(x) {} //NO DEBUG
#endif
#include <stdlib.h>
#include "my402list.h"
#include "maxWeight.h"
#include <stdio.h>
#include <error.h>
#include <string.h>
int main (int argc, char * argv[])
{
	int timeSlot; 
	int packet_c; 
	int finPacket_c; 
	int i,j;
	int dest,pcktArrived;	//destination port of packet arrived
	int weight[numPort][numPort];
	int toDequeue[numPort][numPort]; //an element will be 1,if packet from port corresponding to index of this element is to be served
	int pc[numPort][numPort];	//Packet count in particular queues
	FILE *dat_fp,*wt_fp,*wtN_fp;
	int ld;
	double ldM;
	int tmpC;
	type1 = 0;
	type2 = 0;
	parseConfig();
	My402ListElem *f; //Used for dequeue
	packet *srvdPckt; //Used for dequeue
	memset(&toDequeue,0,sizeof(toDequeue));
	midPoint = RAND_MAX / 2;
	My402List Q[numPort * numPort];
	memset(&Q,0,sizeof(Q));
	
	sprintf(dbgMsg,"W00:\tW01:\tW10:\tW11:\n");
	dbg(dbgMsg);
	if ( (dat_fp = fopen("output.dat","w+")) == NULL)
	{
		fprintf(stderr,"Cannot open file for writing\n");
		perror("fopen::");
		exit(1);
	}
	if ( (wt_fp = fopen("wt.dat","w+")) == NULL)
	{
		fprintf(stderr,"Cannot open file for writing\n");
		perror("fopen::");
		exit(1);
	}	
	if ( (wtN_fp = fopen("wtNorm.dat","w+")) == NULL)
	{
		fprintf(stderr,"Cannot open file for writing\n");
		perror("fopen::");
		exit(1);
	}
	fprintf(dat_fp,"Load[0]\tLoad[1]\tLoad(packets completed / capacity) \t\t packets completed \t packets generated\n");
	srand (time(NULL));
	//load[0] = 1;
	//load[1] = 0.5;
	//for (load[0] = 0; load[0] < 1; load[0] = load[0] + LOADSTEP)	//Load on input port0
	//{
	//for (load[1] = 0; load[1] < 1; load[1] = load[1] + LOADSTEP) //Load on input port1
	//{
	
	InitQueues(&Q);		//Initialise input queues
	packet_c = 0; 
	finPacket_c = 0; 
	pc[0][0] = 0;
	pc[0][1] = 0;
	pc[1][0] = 0;
	pc[1][1] = 0;
	printf("Probabilities: %g %g %g %g\n",p[0][0],p[0][1],p[1][0],p[1][1]);
	for (timeSlot = 1; timeSlot < simTime; timeSlot++)
	{
	//Generate packet
	for (i = 0; i < numPort; i++)	//For all input ports
	{
		dest = -1;
		pcktArrived = -1;
		ld = (int)load[i];
		ldM = load[i] - ld;
		//printf("Input:%d load[%d]:%g ld:%d ldM:%g\n",i,i,load[i],ld,ldM);	
		tmpC = 0;
		while (tmpC < ld)
		{
			if ((pcktArrived = packetArrives(1.0)) == 1)	//Packet arrived
			{
				packet_c++;
				//printf("Input: %d\n",i);
				dest = destPort(p[i][0]);	//Get the destination port
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
				pc[i][pkt->dest]++;	
				//printf("TimeSlot:%d\tp%d\ti/p Port:%d\to/p Port:%d\n",timeSlot,packet_c,i,dest);
				//printf("Packet Arrived Destined to %d\n",dest);
			}
		tmpC++;
		}
		dest = -1;
		pcktArrived = -1;
		if ((pcktArrived = packetArrives(ldM)) == 1)	//Packet arrived
		{
			packet_c++;
			//printf("Input: %d\n",i);
			dest = destPort(p[i][0]);	//Get the destination port
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
			pc[i][pkt->dest]++;	
			//printf("TimeSlot:%d\tp%d\ti/p Port:%d\to/p Port:%d\n",timeSlot,packet_c,i,dest);
			//printf("Packet Arrived Destined to %d\n",dest);
		}
		
	}
	getWeight(weight,&Q,numPort);
	//sprintf(dbgMsg,"W00:%d\tW01:%d\tW10:%d\tW11:%d\n",weight[0][0]/timeSlot,weight[0][1]/timeSlot,weight[1][0]/timeSlot,weight[1][1]/timeSlot);
	fprintf(wt_fp,"%.4g\t\t %.4g\t\t %.4g\t\t %.4g\n",(double)weight[0][0],(double)weight[0][1],(double)weight[1][0],(double)weight[1][1]);
	fprintf(wtN_fp,"%.4g\t\t %.4g\t\t %.4g\t\t %.4g\n",(double)weight[0][0]/timeSlot,(double)weight[0][1]/timeSlot,(double)weight[1][0]/timeSlot,(double)weight[1][1]/timeSlot);
	///sprintf(dbgMsg,"%.4g\t\t %.4g\t\t %.4g\t\t %.4g\n",(double)weight[0][0]/timeSlot,(double)weight[0][1]/timeSlot,(double)weight[1][0]/timeSlot,(double)weight[1][1]/timeSlot);
	//dbg(dbgMsg);
	maxWeight(weight,toDequeue,numPort);	//Do the biparted graph matching
	sprintf(dbgMsg,"D00:%d\tD01:%d\tD10:%d\tD11:%d\n",toDequeue[0][0],toDequeue[0][1],toDequeue[1][0],toDequeue[1][1]);
	//dbg(dbgMsg);

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
				finPacket_c++;
				//printf("TimeSlot:%d\tp%d\tfrom port:%d\tto port:%d\tserviced\n",timeSlot,srvdPckt->pkt_num,i,j);
				free(srvdPckt);
			}
			else
			{
				fprintf(stderr,"Logical flaw. Even if weight is greater than zero the Q[%d][%d] is empty\n",i,j);
				exit(1);
			}
		}
	}
	}
	//printf("Load on port0: %g\nLoad on Port1: %g\n",load[0],load[1]);
	//printf("Total packets generated: %d\n",packet_c);
	//printf("Finished packet counter: %d\n",finPacket_c);
	getWeight(weight,&Q,numPort);
	sprintf(dbgMsg,"W00:%d\tW01:%d\tW10:%d\tW11:%d\n",weight[0][0],weight[0][1],weight[1][0],weight[1][1]);
	//dbg(dbgMsg);
	sprintf(dbgMsg,"PC00: %d PC01: %d PC10: %d PC11: %d\n",pc[0][0],pc[0][1],pc[1][0],pc[1][1]);
	//dbg(dbgMsg);
	fprintf(dat_fp,"%g\t%g\t%g\t%d\t%d\n",load[0],load[1],(double)finPacket_c/(double)(2*simTime),finPacket_c,packet_c);
	if (packet_c < finPacket_c)
		printf("###\n\n");
	//}	//for loop of load[1]
	//}	//for loop of load[0]
	printf("Type1: %d Type2: %d\n",type1, type2);
	fclose(dat_fp);
	return(0);
}
