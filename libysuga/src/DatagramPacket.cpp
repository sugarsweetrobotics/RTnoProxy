
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <string.h>
#include "DatagramPacket.h"


DatagramPacket::DatagramPacket(uint8_t* sendData, uint32_t length, char* address, uint16_t port)
{
  m_Address.sin_family = AF_INET;
  m_Address.sin_port = htons(port);
  m_Address.sin_addr.s_addr = inet_addr(address);
  m_pData = new uint8_t[length];
  m_Length = length;
  memcpy(m_pData, sendData, length);
}



DatagramPacket::~DatagramPacket()
{
  delete m_pData;
}
