#include "Packet.h"
#include "Transport.h"

#include <iostream>
#include <coil/Time.h>
#include <coil/TimeMeasure.h>

#define PACKET_WAITING_TIME 3000 // ms
#define PACKET_SENDING_DELAY 10 // us
#define PACKET_WAITING_DELAY 100 //us
#define PACKET_WAITING_COUNT (PACKET_WAITING_TIME*1000/PACKET_WAITING_DELAY)

//#define DEBUG

const uint32_t RTNO_INFINITE = 0xFFFFFFFF;

using namespace ssr;

Transport::Transport(SerialDevice* pSerialDevice)
{
  m_pSerialDevice = pSerialDevice;
}

Transport::~Transport(void)
{
}

int32_t Transport::read(uint8_t* buffer, uint8_t size, uint32_t wait_usec) 
{
  if (size == 0) {
    return 0;
  }

  coil::TimeMeasure tm;
  tm.tick();
  while(1) {
    if(m_pSerialDevice->getSizeInRxBuffer() >= size) {
      break;
    }
    tm.tack();
    coil::TimeValue tv = tm.interval();
    if (tv.usec() > wait_usec && wait_usec != RTNO_INFINITE) {
      return -TIMEOUT;
    }
  }

  m_pSerialDevice->read(buffer, size);
  return size;
}

int32_t Transport::write(const uint8_t* buffer, const uint8_t size) 
{
  for(uint8_t i = 0;i < size;i++) {
    m_pSerialDevice->write(buffer+i, 1);
    coil::usleep(PACKET_SENDING_DELAY);
  }
  return size;
}


int Transport::send(const RTnoPacket& packet) {
  const uint8_t headers[2] = {0x0a, 0x0a};
  write(headers, 2);
  write(packet.serialize(), packet.getPacketLength());
  uint8_t sum = packet.getSum();
  write(&sum, 1);
  return 0;
}

bool Transport::isNew(const uint32_t wait_usec) {
#ifdef DEBUG
  std::cout << "---Receiving Packet..." << std::endl;
#endif
  uint8_t buf;
  while(1) {
    if (read(&buf, 1, wait_usec) < 0) {
      return false;
    }
    if(buf != 0x0a) {
      continue;
    }

    if(read(&buf, 1, wait_usec) < 0) {
      return false;
    }
    if(buf == 0x0a) {
      break;
    }
  }
#ifdef DEBUG
  std::cout << "----Packet Start Header detected." << std::endl;
#endif
  return true;
}

RTnoPacket Transport::receive(const uint32_t wait_usec/*=RTNO_INFINITE*/)
{
  uint8_t header[PACKET_HEADER_SIZE];
  if(read(header, PACKET_HEADER_SIZE, wait_usec) < 0) {
    throw TimeOutException();
  }

  uint8_t sender[4];
  m_pSerialDevice->getSenderInfo(sender);
  //if(read(sender, 4, wait_usec) < 0) {
  //    throw TimeOutException();
  //  }

  uint8_t data_buffer[256];
  if(read(data_buffer, header[1] , wait_usec) < 0) {
    throw TimeOutException();
  }
  
  RTnoPacket packet(header[0], data_buffer, header[1]);
  uint8_t sum = packet.getSum();
#ifdef DEBUG
  std::cout << "-Received Packet" << std::endl;
  packet.dump();
#endif
  
  uint8_t buf;
  if(read(&buf, 1, wait_usec) < 0) {
    throw TimeOutException();
  }
  
  
  if(sum != buf) {
    std::cout << "--CheckSum Error. Packet Dump" << std::endl;
    packet.dump();
    //return -CHECKSUM_ERROR;
    throw CheckSumException();
  }
  
  return packet;
}
