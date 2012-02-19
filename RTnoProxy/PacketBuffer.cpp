
#include <string.h>
#include "Packet.h"
#include "PacketBuffer.h"


PacketBuffer::PacketBuffer(int8_t interface,
			   int8_t* address,
			   int8_t sourcePortIndex,
			   int8_t targetPortIndex,
			   int8_t* pData,
			   int8_t size)
{
  m_Interface = interface;
  m_DataLength = size;
  m_pData = NULL;
  if(size != 0) {
    m_pData = new int8_t[m_DataLength];
    memcpy(m_pData, pData, size);
  }
  m_TargetPortIndex = targetPortIndex;
  m_SourcePortIndex = sourcePortIndex;
  memcpy(m_Address, address, SOURCE_ADDR_SIZE);
}

PacketBuffer::PacketBuffer(int8_t* packetBuffer)// :
  //  this(packetBuffer[0], packetBuffer+2, packetBuffer[6], packetBuffer[7], packetBuffer+8, packetBuffer[1])
{
  m_Interface = packetBuffer[PACKET_INTERFACE];
  m_DataLength = packetBuffer[DATA_LENGTH];
  m_pData = NULL;
  if(m_DataLength != 0) {
    m_pData = new int8_t[m_DataLength];
    memcpy(m_pData, packetBuffer + DATA_START_ADDR, m_DataLength);
  }
  m_TargetPortIndex = packetBuffer[TARGET_PORT];
  m_SourcePortIndex = packetBuffer[SOURCE_PORT];
  memcpy(m_Address, packetBuffer + SOURCE_ADDR, SOURCE_ADDR_SIZE);

}


PacketBuffer::~PacketBuffer()
{
  delete[] m_pData;
}

int8_t PacketBuffer::sum(int8_t* pData, int8_t size) {
  uint8_t val = 0;
  for(int i = 0;i < size;i++) {
    val += *((uint8_t*)pData+i);
  }
  return  *((int8_t*)(&val));
}

uint8_t PacketBuffer::serialize(int8_t* packetBuffer, uint8_t maxSize)
{
  packetBuffer[0] = m_Interface;
  packetBuffer[1] = m_DataLength;
  memcpy(packetBuffer+2, m_Address, SOURCE_ADDR_SIZE);
  packetBuffer[6] = m_SourcePortIndex;
  packetBuffer[7] = m_TargetPortIndex;
  if(m_DataLength!=0) {
    memcpy(packetBuffer+8, m_pData, m_DataLength);
  }
  packetBuffer[8+m_DataLength] = sum(packetBuffer, 8+m_DataLength);
  return PACKET_HEADER_SIZE + m_DataLength + 1; // +1 is checksum
}
