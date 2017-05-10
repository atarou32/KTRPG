
#ifndef ACCOUNT_H
#include "account.h"
#endif

#define MAX_PACKET_SIZE 4096
#define MAX_FLAGMENT_PACKET_CONT 32

#define FPACKET_CONT_INUSE 1
#define FPACKET_CONT_NOUSE 0
#define FPACKET_CONT_CANTUSE 1
#define FPACKET_CONT_CANUSE 0


#define PACKET_MARK_START 0xFE23
#define PACKET_MARK_END 0xF329

char fpacket_cont[MAX_FLAGMENT_PACKET_CONT][MAX_PACKET_SIZE];
char fpacket_cont_inuse[MAX_FLAGMENT_PACKET_CONT];

void initPacket();
void releasePacket();

int hasFPacketCont();
int getFPacketContIndex();

void analyzePacket(account* sender, char* packet, int plen);
void doPacket(account* sender, char* analyzed_packet_body,int plen, void (*ent)(account*,int, char*,int));
void sendPacket(account* sendto, char* body, int blen, int packet_type);

