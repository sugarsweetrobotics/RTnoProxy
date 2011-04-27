#include <iostream>
#include <coil/Time.h>
#include "ReceivePacket.h"

using namespace org::ysuga;

int ReceivePacket(SerialPort* pSerial, char* packet) {
  int counter = 0;
  unsigned char sum = 0;
  while(pSerial->GetSizeInRxBuffer() < PACKET_HEADER_SIZE) {
	  coil::usleep(PACKET_WAITING_DELAY);

    counter++;
    if(counter == PACKET_WAITING_COUNT) {
      std::cout << "ReceivePacketHeader::Timeout" << std::endl;
      return -TIMEOUT;
    }
  }

  pSerial->Read(packet, PACKET_HEADER_SIZE);
  sum += packet[PACKET_INTERFACE];
  sum += packet[DATA_LENGTH];

  counter = 0;
  while(pSerial->GetSizeInRxBuffer() < packet[DATA_LENGTH]+1) {
	  coil::usleep(PACKET_WAITING_DELAY);
    counter++;
    if(counter == PACKET_WAITING_COUNT) {
      std::cout << "ReceivePacketData::Timeout" << std::endl;
      return -DATA_TIMEOUT;
    }
  }

  pSerial->Read(&(packet[PACKET_HEADER_SIZE]), packet[DATA_LENGTH]+1);
  for(int i = 0;i < packet[DATA_LENGTH];i++) {
    sum += packet[PACKET_HEADER_SIZE+i];
  }

  if(sum != *((unsigned char*)packet+PACKET_HEADER_SIZE+packet[DATA_LENGTH])) {
    std::cout << "CheckSum Error." << std::endl;
    return -CHECKSUM_ERROR;
  }
  return PACKET_HEADER_SIZE + packet[DATA_LENGTH] + 1;
}

/*
int read_string(char* buffer, int flag=1) {
  int counter = 0;
  for(int i = 0;i < 100;i++) {
    if(Serial.available() > 0) {
      buffer[counter] = Serial.read();
      if(buffer[counter] == '^') {
	buffer[counter] = 0;
	Serial.print("AO^");
	return counter+1;
      }
      counter++;
    } 
    delayMicroseconds(100);
  }
  if(flag)
  Serial.print("AE^");
  return -1;
}

*/
