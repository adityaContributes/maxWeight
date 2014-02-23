char dbgMsg[1024];
int midPoint;
typedef struct packet
{
	int pkt_num;
	int dest;	//Destination port
	int timeEnter;	//time slot at which it entered the input queue 
	int timeLeft;	//time slot at which it left the queue
}packet;
