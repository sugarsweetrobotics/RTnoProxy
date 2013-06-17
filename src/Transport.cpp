#include "Transport.h"
#include "Packet.h"
#include <iostream>
#include <coil/Time.h>

#define PACKET_WAITING_TIME 3000 // ms
#define PACKET_SENDING_DELAY 10 // us
#define PACKET_WAITING_DELAY 100 //us
#define PACKET_WAITING_COUNT (PACKET_WAITING_TIME*1000/PACKET_WAITING_DELAY)

using namespace net::ysuga;

Transport::Transport(SerialDevice* pSerialDevice)
{
	m_pSerialDevice = pSerialDevice;
}

Transport::~Transport(void)
{
}


int Transport::SendPacket(unsigned char interFace, unsigned char size, unsigned char* packet_data)
{
	unsigned char sum = 0;
	m_pSerialDevice->Write(&interFace, 1);
	sum += interFace;
	coil::usleep(PACKET_SENDING_DELAY);
	m_pSerialDevice->Write(&size, 1);
	sum += size;
	coil::usleep(PACKET_SENDING_DELAY);

	unsigned char sender[4] = {'U', 'A', 'R', 'T'};
	for(int i = 0;i < 4;i++) {
		sum += sender[i];
		m_pSerialDevice->Write(sender+i, 1);
		coil::usleep(PACKET_SENDING_DELAY);
	}

	for(int i = 0;i < size;i++) {
		sum += packet_data[i];
		m_pSerialDevice->Write(packet_data+i, 1);
		coil::usleep(PACKET_SENDING_DELAY);
	}
	m_pSerialDevice->Write(&sum, 1);
	return PACKET_HEADER_SIZE + size + 1;
}

int Transport::_Wait(unsigned char buffer_size) {
	int counter = 0;
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

int Transport::ReceivePacket(unsigned char* packet) {
	unsigned char sum = 0;
	if(m_pSerialDevice->GetSizeInRxBuffer() == 0) {
		return 0;
	}

	if(_Wait(PACKET_HEADER_SIZE) < 0) return -TIMEOUT;
	m_pSerialDevice->Read(packet, PACKET_HEADER_SIZE);
	sum += packet[PACKET_INTERFACE];
	sum += packet[DATA_LENGTH];

	if(_Wait(4) < 0) return -TIMEOUT;
	unsigned char sender[4];
	m_pSerialDevice->Read(sender, 4);
	for(int i = 0;i < 4;i++) {
		sum += sender[i];
	}

	if(_Wait(packet[DATA_LENGTH]+1) < 0) return -TIMEOUT;
	m_pSerialDevice->Read(&(packet[PACKET_HEADER_SIZE]), packet[DATA_LENGTH]+1);
	for(int i = 0;i < packet[DATA_LENGTH];i++) {
		sum += packet[PACKET_HEADER_SIZE+i];
	}

	if(sum != *((unsigned char*)packet+PACKET_HEADER_SIZE+packet[DATA_LENGTH])) {
		std::cout << "CheckSum Error." << std::endl;
		return -CHECKSUM_ERROR;
	}

	return PACKET_HEADER_SIZE + packet[DATA_LENGTH] + 1;
}
