#include "Packet.h"
#include "Transport.h"

#include <iostream>
#include <coil/Time.h>
#include <coil/TimeMeasure.h>

#define PACKET_WAITING_TIME 3000 // ms
#define PACKET_SENDING_DELAY 10 // us
#define PACKET_WAITING_DELAY 100 //us
#define PACKET_WAITING_COUNT (PACKET_WAITING_TIME*1000/PACKET_WAITING_DELAY)


const uint32_t INFINITE = 0xFFFFFFFF;

using namespace net::ysuga;

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
    if(m_pSerialDevice->GetSizeInRxBuffer() >= size) {
      break;
    }
    tm.tack();
    coil::TimeValue tv = tm.interval();
    if (tv.usec() > wait_usec && wait_usec != INFINITE) {
      return -TIMEOUT;
    }
  }

  m_pSerialDevice->Read(buffer, size);
  return size;
}

int32_t Transport::write(const uint8_t* buffer, const uint8_t size) 
{
  for(uint8_t i = 0;i < size;i++) {
    m_pSerialDevice->Write(buffer+i, 1);
    coil::usleep(PACKET_SENDING_DELAY);
  }
  return size;
}


int Transport::send(const Packet& packet) {
  const uint8_t headers[2] = {0x0a, 0x0a};
  write(headers, 2);

  write(packet.getData(), packet.getDataLength());

  uint8_t sum = packet.getSum();

  write(&sum, 1);
  return 0;
}

bool Transport::isNew()
{
#ifdef DEBUG
  std::cout << "---Receiving Packet..." << std::endl;
#endif
  uint8_t buf;
  while(1) {
    if (read(&interface, 1, wait_usec) < 0) {
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

Packet Transport::receive(const uint32_t wait_usec/*=INFINITE*/)
{
  uint8_t interface, length;
  if(read(packet, PACKET_HEADER_SIZE, wait_usec) < 0) {
    throw TimeOutException();
  }

  uint8_t sender[4];
  if(read(sender, 4, wait_usec) < 0) {
    throw TimeOutException();
  }
#ifdef DEBUG
  std::cout << "----Interface   = " << (int)packet[0] << std::endl;
  std::cout << "----Data Length = " << (int)packet[DATA_LENGTH] << std::endl;
#endif

  uint8_t data_buffer[256];
  if(read(data_buffer, length, wait_usec) < 0) {
    throw TimeOutException();
  }
  
  uint8_t sum = 0x0a + 0x0a + sender[0] + sender[1] + sender[2] + sender[3];
  for(uint8_t i = 0;i < PACKET_HEADER_SIZE + packet[DATA_LENGTH];i++) {
    sum += packet[i];
  }
  
  if((ret=read(&buf, 1, wait_usec)) < 0) {
    throw TimeOutException();
  }
  
  
  if(sum != buf) {
    std::cout << "--CheckSum Error. Packet Dump(len=" << (int)packet[DATA_LENGTH] << "):" << std::ends;
    std::cout << std::hex << (int)packet[0] << " " << (int)packet[1] << " ";
    for (int i = 0;i < 4;i++) {
      std::cout << (int)sender[i] << " " << std::ends;
    }
    std::cout << std::endl;
    for (int i = 0;i < packet[DATA_LENGTH];i++) {
      std::cout << std::hex << (int)packet[2+i] << " " << std::ends;
    }
    std::cout << std::endl << "sum=" << std::dec << sum << "/" << buf << ":received." << std::endl;
    //return -CHECKSUM_ERROR;
    throw CheckSumException();
  }
  
  return PACKET_HEADER_SIZE + packet[DATA_LENGTH] + 1;
}
