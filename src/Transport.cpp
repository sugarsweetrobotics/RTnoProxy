#include "Transport.h"
#include "Packet.h"
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


int Transport::SendPacket(unsigned char interFace, unsigned char size, unsigned char* packet_data)
{
  const uint8_t headers[2] = {0x0a, 0x0a};
  write(headers, 2);

  write(&interFace, 1);
  write(&size, 1);

  uint8_t sender[4] = {'U', 'A', 'R', 'T'};
  write(sender, 4);

  write(packet_data, size);
  
  uint8_t sum = 0x0a + 0x0a + interFace + size + sender[0] + sender[1] + sender[2] + sender[3];
  for(uint8_t i = 0;i < size;i++) {
    sum += packet_data[i];
  }
  write(&sum, 1);
  return PACKET_HEADER_SIZE + size + 1;
}

int Transport::_Wait(unsigned char buffer_size) {
  int counter = 0;
  //std::cout << "Waiting .....:" << m_pSerialDevice->GetSizeInRxBuffer() << std::endl;
	while(m_pSerialDevice->GetSizeInRxBuffer() < buffer_size) {

		coil::usleep(PACKET_WAITING_DELAY);
		counter++;
		if(counter == PACKET_WAITING_COUNT) {
			std::cout << "ReceivePacket::Timeout" << std::endl;
			throw "TimeOut";
		}
	}
	return buffer_size;
}

int Transport::IsReceived() {
	return m_pSerialDevice->GetSizeInRxBuffer();
}

int32_t Transport::ReceivePacket(uint8_t* packet, uint32_t wait_usec) {
  uint8_t buf;
  int32_t ret;
#ifdef DEBUG
  std::cout << "---Receiving Packet..." << std::endl;
#endif
  while(1) {
    if ((ret=read(&buf, 1, wait_usec)) < 0) {
      return 0;//ret;
    }
    if(buf != 0x0a) {
      continue;
    }

    if((ret=read(&buf, 1, wait_usec)) < 0) {
      return 0;//ret;
    }
    if(buf == 0x0a) {
      break;
    }
  }
#ifdef DEBUG
  std::cout << "----Packet Start Header detected." << std::endl;
#endif
  if((ret=read(packet, PACKET_HEADER_SIZE, wait_usec)) < 0) {
    return ret;
  }
  uint8_t sender[4];
  if((ret=read(sender, 4, wait_usec)) < 0) {
    return ret;
  }
  //#ifdef DEBUG
  std::cout << "----Interface   = " << (int)packet[0] << std::endl;
  std::cout << "----Data Length = " << (int)packet[DATA_LENGTH] << std::endl;
  //#endif
  if((ret=read(packet+PACKET_HEADER_SIZE, packet[DATA_LENGTH], wait_usec)) < 0) {
    return ret;
  }

  uint8_t sum = 0x0a + 0x0a + sender[0] + sender[1] + sender[2] + sender[3];
  for(uint8_t i = 0;i < PACKET_HEADER_SIZE + packet[DATA_LENGTH];i++) {
    sum += packet[i];
  }
  
  if((ret=read(&buf, 1, wait_usec)) < 0) {
    return ret;
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
    return -CHECKSUM_ERROR;
  }
  
  return PACKET_HEADER_SIZE + packet[DATA_LENGTH] + 1;
}
