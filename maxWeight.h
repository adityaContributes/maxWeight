#define numPort 2
#define LOADSTEP 0.05
char dbgMsg[1024];
int midPoint;
typedef struct packet
{
	int pkt_num;
	int dest;	//Destination port
	int timeEnter;	//time slot at which it entered the input queue 
	int timeLeft;	//time slot at which it left the queue
}packet;
double load[numPort];//The load on the switch
double p[numPort][numPort]; //probability to the destination of packet
int simTime;
void parseConfig();
int type1;
int type2;
