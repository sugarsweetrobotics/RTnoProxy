#pragma once

#include "libysuga.h"
#include <stdint.h>
#include <sys/types.h>
#include <netinet/in.h>


namespace net {
  namespace ysuga {
class LIBYSUGA_API DatagramPacket {
 private:
  struct sockaddr_in m_Address;
  uint8_t* m_pData;
  uint32_t  m_Length;


 public:
  DatagramPacket(uint8_t* sendData, uint32_t length, char* address, uint16_t port);
  virtual ~DatagramPacket();

 public:
  uint8_t* getData() {return m_pData;}
  uint32_t getLength() {return m_Length;}
  struct sockaddr* getAddress() {return (struct sockaddr*)&m_Address;}
  uint32_t getAddressSize() {return sizeof(m_Address); }
};
  }
}
