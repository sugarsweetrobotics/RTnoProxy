#ifndef RECEIVE_PACKET_HEADER_INCLUDED
#define RECEIVE_PACKET_HEADER_INCLUDED

#include "SerialPort.h"
#include "Packet.h"

int ReceivePacket(org::ysuga::SerialPort* pSerialPort, unsigned char* packet);

#endif
