#include <unistd.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

#include <iostream>
#include <exception>
#include <string.h>
#include "DatagramSocket.h"

#define BUFFSIZE 4096
using namespace net::ysuga;
DatagramSocket::DatagramSocket()
{
  m_Socket = socket(AF_INET, SOCK_DGRAM, 0);
  m_Bind = false;
}


DatagramSocket::DatagramSocket(uint16_t port)
{
  m_Socket = socket(AF_INET, SOCK_DGRAM, 0);
  
  std::cout << "DatagramSocket::init(" << port << ")" << std::endl;
  memset(&m_Address, 0, sizeof(m_Address));
  m_Address.sin_family = AF_INET;
  m_Address.sin_port = htons(port);
  m_Address.sin_addr.s_addr = htonl(INADDR_ANY);
  if(bind(m_Socket, (struct sockaddr*)&m_Address, sizeof(m_Address)) < 0) {
    std::cout << "Bind Failed." << std::endl;
    perror("BIND");
    throw std::exception();
  }
  std::cout << "Bind OK." << std::endl;
  m_Bind = true;
}
    
DatagramSocket::~DatagramSocket()
{
  close(m_Socket);
}


void DatagramSocket::send(DatagramPacket* pPacket)
{
  sendto(m_Socket, pPacket->getData(), pPacket->getLength(), 0, pPacket->getAddress(),
	 pPacket->getAddressSize());
}

DatagramPacket* DatagramSocket::receive()
{
  uint8_t buf[BUFFSIZE];
  struct sockaddr_in address;
  memset(&address, 0, sizeof(address));
  socklen_t addressSize = sizeof(address);
  int size = recvfrom(m_Socket, &buf, BUFFSIZE, 0, (struct sockaddr*)&address, &addressSize);
  return new DatagramPacket((uint8_t*)&buf, size, inet_ntoa(address.sin_addr), ntohs(address.sin_port));
}

