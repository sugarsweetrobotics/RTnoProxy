#pragma once

#include <sys/types.h>

#include "DatagramPacket.h"
class DatagramSocket {
 private:
  int m_Socket;
  struct sockaddr_in m_Address;
  int m_Bind;
 public:
  DatagramSocket();
  DatagramSocket(uint16_t port);
  ~DatagramSocket();


 public:
  void send(DatagramPacket* pPacket);
  DatagramPacket* receive(DatagramPacket* pPacket);
};
