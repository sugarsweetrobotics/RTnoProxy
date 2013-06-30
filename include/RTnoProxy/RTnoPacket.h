

#pragma once
#include <stdint.h>
#include <string.h> // memcpy


#include "Packet.h"

#define PACKET_HEADER_SIZE 2
#define PACKET_SENDER_INFO_LENGTH 4

namespace ssr {

  class RTnoPacket {
  private:
    uint8_t *m_pData;

  public:
    uint8_t getInterface() {return m_pData[0];}
    uint8_t getDataLength() { return m_pData[1];}
    uint8_t getSenderInfo() {return m_pData + PACKET_HEADER_SIZE;}
    uint8_t getPacketLength() { return PACKET_HEADER_SIZE + PACKET_SENDER_INFO_LENGTH + m_pData[1]; }
    uint8_t *getData() {return m_pData + PACKET_HEADER_SIZE;}
    uint8_t *serialize() {return m_pData;}
    uint8_t getSum() {uint8_t sum = 0; for(uint32_t i = 0;i < getPacketLength();i++) sum+=m_pData[i]; return sum;}

  private:
    void initialize(const uint8_t interFace, const uint8_t* sender, const uint8_t* data = NULL, const uint8_t size = 0) {
      delete m_pData;
      m_pData = new uint8_t[size + PACKET_HEADER_SIZE + PACKET_SENDER_INFO_LENGTH];
      m_pData[0] = interFace;
      m_pData[1] = size;
      memcpy(m_pData + PACKET_HEADER_SIZE, sender, PACKET_SENDER_INFO_LENGTH);
      if(size > 0) {
	memcpy(m_pData + PACKET_HEADER_SIZE + PACKET_SENDER_INFO_LENGTH,
	       data, size);
      }
    }

  public:
  RTnoPacket(const uint8_t* p, const uint8_t size) : m_pData(NULL) {
      uint8_t size = m_pData[1] + PACKET_HEADER_SIZE + PACKET_SENDER_INFO_LENGTH;
      m_pData = new uint8_t[size];
      memcpy(m_pData, p, size);
    }
    
  RTnoPacket(const uint8_t interFace, const uint8_t* sender, const uint8_t* data = NULL, const uint8_t size = 0) :
    m_pData(NULL){
      initialize(interFace, sender, data, size);
    }
    
  RTnoPacket(const RTnoPacket& p) : m_pData(NULL) {
      initialize(p.getInterface(), p.getSenderInfo(), p.getData(), p.getDataLength());
    }


  public:

    void operator=(const RTnoPacket& p) {
      initialize(p.getInterface(), p.getSenderInfo(), p.getData(), p.getDataLength());
    }
      
    ~RTnoPacket() {
      delete m_pData;
    }
    
  public:
    
    
  };
}
