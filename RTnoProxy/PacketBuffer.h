#ifndef PACKET_BUFFER_HEADER_INCLUDED
#define PACKET_BUFFER_HEADER_INCLUDED

#include <stdint.h>

#define SOURCE_ADDR_SIZE 4

class PacketBuffer {
 private:
  int8_t m_Interface;
  int8_t m_DataLength;
  int8_t m_Address[SOURCE_ADDR_SIZE];
  int8_t m_TargetPortIndex;
  int8_t m_SourcePortIndex;
  int8_t *m_pData;


 public:
  PacketBuffer(int8_t interface, int8_t* address, int8_t sourcePortIndex, int8_t targetPortIndex, int8_t* pData, int8_t size);
  PacketBuffer(int8_t* packetBuffer);
  virtual ~PacketBuffer();

 public:
  int8_t getInterface() {return m_Interface;}
  int8_t getDataLength() {return m_DataLength;}
  int8_t* getSourceAddress() {return m_Address;}
  int8_t getSourcePortIndex() {return m_SourcePortIndex;}
  int8_t getTargetPortIndex() {return m_TargetPortIndex;}
  int8_t* getDataBuffer() {return m_pData;}

 private:
  int8_t sum(int8_t* pData, int8_t size);
 public:
  uint8_t serialize(int8_t* packetBuffer, uint8_t maxSize);
};

#endif // #ifndef PACKET_BUFFER_HEADER_INCLUDED
