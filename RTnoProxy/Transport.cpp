#include "Transport.h"
#include "Packet.h"
#include <iostream>
#include <coil/Time.h>

#define PACKET_WAITING_TIME 3000 // ms
#define PACKET_SENDING_DELAY 10 // us
#define PACKET_WAITING_DELAY 100 //us
#define PACKET_WAITING_COUNT (PACKET_WAITING_TIME*1000/PACKET_WAITING_DELAY)

using namespace net::ysuga;

Transport::Transport(SerialDevice* pSerialDevice)
{
  m_pPacketBuffer = NULL;
  m_pSerialDevice = pSerialDevice;
}

Transport::~Transport(void)
{
  delete m_pPacketBuffer;
}

int Transport::SendPacket(char* address, PacketBuffer* packet) 
{
  int8_t packetBuffer[256];
  uint8_t size = packet->serialize(packetBuffer, 255);
  m_pSerialDevice->Write(packetBuffer, size);
  return 0;
}


/*
int Transport::SendPacket(unsigned char interFace,
			  unsigned char size, unsigned char* packet_data)
{
  unsigned char sum = 0;
  m_pSerialDevice->Write(&interFace, 1);
  sum += interFace;
  coil::usleep(PACKET_SENDING_DELAY);
  m_pSerialDevice->Write(&size, 1);
  sum += size;
  coil::usleep(PACKET_SENDING_DELAY);
  
  unsigned char sender[4] = {'U', 'A', 'R', 'T'};
  for(int i = 0;i < 4;i++) {
    sum += sender[i];
    m_pSerialDevice->Write(sender+i, 1);
    coil::usleep(PACKET_SENDING_DELAY);
  }
  
  for(int i = 0;i < size;i++) {
    sum += packet_data[i];
    m_pSerialDevice->Write(packet_data+i, 1);
    coil::usleep(PACKET_SENDING_DELAY);
  }
  m_pSerialDevice->Write(&sum, 1);
  return PACKET_HEADER_SIZE + size + 1;
}
*/
int Transport::_Wait(unsigned char buffer_size) {
  int counter = 0;
  while(m_pSerialDevice->GetSizeInRxBuffer() < buffer_size) {
    coil::usleep(PACKET_WAITING_DELAY);
    counter++;
    if(counter == PACKET_WAITING_COUNT) {
      std::cout << "ReceivePacket::Timeout" << std::endl;
      throw "TimeOut";
    }
  }
  return buffer_size;
}

int Transport::IsReceived() {
  return m_pSerialDevice->GetSizeInRxBuffer();
}

PacketBuffer *Transport::ReceivePacket() {
  int8_t packetBuffer[256];
  int ret = ReceivePacket((unsigned char*)packetBuffer);
  if(ret == 0) return NULL;
  if(ret < 0) return NULL;
  delete m_pPacketBuffer;
  m_pPacketBuffer = new PacketBuffer(packetBuffer);
  return m_pPacketBuffer;
}

int Transport::ReceivePacket(unsigned char* packet) {
  unsigned char sum = 0;
  if(m_pSerialDevice->GetSizeInRxBuffer() == 0) {
    return 0;
  }
  
  if(_Wait(PKT_HEADER_SIZE) < 0) {
    std::cout << "ReceivePacket TIMEOUT. (RxBuffer is not empty but data is smaller than Header)" << std::endl;
    return RTNO_RTC_PACKET_TIMEOUT;
  }
  m_pSerialDevice->Read(packet, PKT_HEADER_SIZE);

  if(_Wait(packet[PKT_ADDR_DATA_LENGTH]+1) < 0) return -RTNO_RTC_PACKET_TIMEOUT;
  m_pSerialDevice->Read(&(packet[PKT_HEADER_SIZE]), packet[PKT_ADDR_DATA_LENGTH]+1);
  for(int i = 0;i < PKT_HEADER_SIZE + packet[PKT_ADDR_DATA_LENGTH];i++) {
    sum += packet[i];
  }
  
  if(sum != packet[PKT_HEADER_SIZE+packet[PKT_ADDR_DATA_LENGTH]]) {
    std::cout << "CheckSum Error." << std::endl;
    return RTNO_RTC_CHECKSUM_ERROR;
  }
  
  return PKT_HEADER_SIZE + packet[PKT_ADDR_DATA_LENGTH] + 1;
}
