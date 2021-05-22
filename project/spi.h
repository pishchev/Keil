#include <stm32f0xx.h>
#include <stdbool.h>

void initSPI(void);

typedef struct _Packet
{
	bool data[8][8];
	int sendingStage;
}Packet;

void ConstrPacket(Packet* packet);
void stagingPacket(Packet* packet);
void loadPacket(Packet* packet);
void sendPacket(Packet* packet);