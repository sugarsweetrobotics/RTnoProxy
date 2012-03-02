#include "EtherUdp.h"
#include <iostream>


using namespace net::ysuga;

EtherUdp::EtherUdp(const char* ipAddress, int port)
{
  m_ipAddress = std::string(ipAddress);
  m_Port = port;
  m_pSocket = new DatagramSocket();
  m_pServerSocket = new DatagramSocket(port);
  coil::Task::activate();
}


EtherUdp::~EtherUdp()
{
  m_Endflag = 1;
  coil::Task::wait();
  delete m_pSocket;
  delete m_pServerSocket;
}

void EtherUdp::FlushRxBuffer()
{

}

/**
 * @brief flush transmit buffer.
 * @return zero if success.
 */
void EtherUdp::FlushTxBuffer()
{

}

/**
 * @brief Get stored datasize of in Rx Buffer
 * @return Stored Data Size of Rx Buffer;
 */
int EtherUdp::GetSizeInRxBuffer()
{
  if(m_pPacket != NULL) {
    return m_pPacket->getLength() - m_DataPointer;
  }
  return 0;
}

/**
 * @brief write data to Tx Buffer of Serial Port.
 *
 */
int EtherUdp::Write(const void* src, const unsigned int size)
{
  //  std::cout << "EtherUdp::Write()" << std::endl;
  DatagramPacket packet((uint8_t*)src, size, (char*)m_ipAddress.c_str(), m_Port);
  m_pSocket->send(&packet);
  return size;
}

/**
 * @brief read data from RxBuffer of Serial Port 
 */
int EtherUdp::Read(void *dst, const unsigned int size)
{
  //  std::cout << "EtherUdp::Read()" << std::endl;
  if(m_pPacket == NULL) {
    return -1;
  }

  uint8_t* pData = m_pPacket->getData();
  memcpy((char*)dst, pData + m_DataPointer, size);
  m_DataPointer += size;
  return size;
}

void EtherUdp::ReceivePacket() {
  //  std::cout << "EtherUdp::ReceivePacket()" << std::endl;
  if(m_Queue.size() > 0) {
    m_pPacket = m_Queue.front();
    m_DataPointer = 0;
    m_Queue.pop();
  } else {
    m_pPacket = NULL;
  }
}

int EtherUdp::svc(void)
{
  std::cout << "Starting EtherUdp::svc loop" << std::endl;
  m_Endflag = 0;
  while(!m_Endflag) {
    DatagramPacket* pPacket = m_pServerSocket->receive();
    //    std::cout << "Receve UDP Packet." << std::endl;
    m_Queue.push(pPacket);
  }
  return 0;
}
