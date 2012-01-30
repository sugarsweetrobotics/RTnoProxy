#include "stdafx.h"

#include "RTnoRTObjectWrapper.h"
#include "Packet.h"
#include "RTnoProtocol.h"

#include <iostream>

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef TRUE
#define TRUE (!FALSE)
#endif

using namespace net::ysuga;

RTnoProtocol::RTnoProtocol(RTnoRTObjectWrapper* pRTObject, Transport *pTransport)
{
	m_SendBusy = FALSE;
	m_pRTObjectWrapper = pRTObject;
	m_pTransport = pTransport;
}

RTnoProtocol::~RTnoProtocol(void)
{
}

std::string RTnoProtocol::GetStringFromPacket(const unsigned char* start_adr, int length)
{
	char* cbuf = new char[length+1];
	memcpy(cbuf, start_adr, length);
	cbuf[length] = 0;
	std::string ret(cbuf);
	delete cbuf;
	return ret;
}

int RTnoProtocol::GetRTnoProfile(RTnoProfile *profile)
{
	std::cout << "-RTnoProtocol::getRTnoProfile() called." << std::endl;
	unsigned char packet_buffer[MAX_PACKET_SIZE];

	m_pTransport->SendPacket(GET_PROFILE, 0, NULL);
	while(1) {
		int retval = m_pTransport->ReceivePacket(packet_buffer);
		if(retval == 0) continue;
		else if(retval < 0) {
			std::cout << "--Packet Receive Failed." << std::endl;
			return -1;
		}

		std::cout << "--Packet Received." << std::endl;

		std::string strbuf;
		switch(packet_buffer[PACKET_INTERFACE]) {
		case GET_PROFILE: // Return Code.
			if(packet_buffer[DATA_START_ADDR] == RTNO_OK) {
				std::cout << "--RTnoProtocol::getRTnoProfile() Succeeded." << std::endl;
				return 0;
			} else {
				std::cout << "--RTnoProtocol::getRTnoProfile() Failed." << std::endl;
				return -2;
			}
			break;
		case ADD_INPORT: 
			strbuf = GetStringFromPacket(packet_buffer+DATA_START_ADDR+1, packet_buffer[DATA_LENGTH]-1);
			profile->AddInPort(packet_buffer[DATA_START_ADDR], strbuf.c_str());
		  break;
		case ADD_OUTPORT:
			strbuf = GetStringFromPacket(packet_buffer+DATA_START_ADDR+1, packet_buffer[DATA_LENGTH]-1);
			profile->AddOutPort(packet_buffer[DATA_START_ADDR], strbuf.c_str());
		  break;
		default:
			std::cout << "Unknown Command (" << packet_buffer[PACKET_INTERFACE] << ")" << std::endl;
			return -1;
		}
	}
}



unsigned char RTnoProtocol::GetRTnoStatus()
{	
  //	unsigned char packet_buffer[MAX_PACKET_SIZE];
	std::cout << "-RTnoProtocol::GetRTnoStatus() called." << std::endl;
//	unsigned char packet_buffer[MAX_PACKET_SIZE];

	m_pTransport->SendPacket(GET_STATUS, 0, NULL);
	return ReceiveReturnCode(GET_STATUS);
}

unsigned char RTnoProtocol::GetRTnoExecutionContextType()
{
	std::cout << "-RTnoProtocol::GetRTnoExecutionContextType() called." << std::endl;
	//	unsigned char packet_buffer[MAX_PACKET_SIZE];
	
	m_pTransport->SendPacket(GET_CONTEXT, 0, NULL);
	return ReceiveReturnCode(GET_CONTEXT);
}

int RTnoProtocol::ReceiveReturnCode(unsigned char intf) {
	unsigned char packet_buffer[MAX_PACKET_SIZE];

	while(1) {
		int ret = m_pTransport->ReceivePacket(packet_buffer);
		if(ret == 0) continue;
		if(ret == -TIMEOUT) {
			std::cout << "---Timeout Error." << std::endl;
			return -1; // Timeout or Checksum error.
		} else if(ret == -CHECKSUM_ERROR) {
			std::cout << "---Checksum Error." << std::endl;
			return -1;
		}

		if(packet_buffer[PACKET_INTERFACE] == SEND_DATA) {
			m_SendBusy = FALSE;
			continue;
		}

		if(packet_buffer[PACKET_INTERFACE] == RECEIVE_DATA) {
			ReceiveData(packet_buffer);
			continue;
		}

		if(packet_buffer[PACKET_INTERFACE] != intf) {
			std::cout << "---Unknown Packet Interface [" << packet_buffer[PACKET_INTERFACE] << "] expected:" << intf << std::endl;
			return -2;
		}

		return packet_buffer[DATA_START_ADDR];
	}
}


int RTnoProtocol::ActivateRTno()
{
  std::cout << "-RTnoProtocol::ActivateRTno() called....";
  m_pTransport->SendPacket(ACTIVATE, 0, NULL);
  int ret = ReceiveReturnCode(ACTIVATE);
  if(ret != RTNO_OK) {
    std::cout << "Failed." << std::endl;
    return ret;
  }
  std::cout << "Succeed." << std::endl;
  return 0;
}

int RTnoProtocol::DeactivateRTno()
{
  std::cout << "-RTnoProtocol::DeactivateRTno() called....";
  m_pTransport->SendPacket(DEACTIVATE, 0, NULL);
  int ret = ReceiveReturnCode(DEACTIVATE);
  if(ret != RTNO_OK) {
    std::cout << "Failed." << std::endl;
    return ret;
  }
  std::cout << "Succeed." << std::endl;
  return 0;
}


int RTnoProtocol::SendData(const char* portName, const unsigned char* data, int length)
{
	while(this->m_SendBusy) {
		coil::TimeValue tv(1000.0); // 1 ms;
		coil::sleep(tv);
	}
	//std::cout << "-RTnoProtocol::SendData() called."<< std::endl;
	unsigned char packet_buffer[MAX_PACKET_SIZE];

	int namelen = strlen(portName);
	packet_buffer[0] = namelen;
	packet_buffer[1] = length;
	memcpy(packet_buffer+2, portName, namelen);
	memcpy(packet_buffer+2+namelen, data, length);
	m_SendBusy = TRUE;
	m_pTransport->SendPacket(SEND_DATA, length + namelen + 2, packet_buffer);

	/*
	while(1) {
		int ret = m_pTransport->ReceivePacket(packet_buffer);
		if(ret == 0) {
			continue;
		} else if(ret < 0) {
			return -1;
		}

		if(packet_buffer[PACKET_INTERFACE] != SEND_DATA) {
			return -2;
		}

		if(packet_buffer[DATA_START_ADDR] != RTNO_OK) {
			return -3;
		}
		return 0;
	}
	*/
	return 0;
}


int RTnoProtocol::SendExecuteTrigger(void)
{
	return m_pTransport->SendPacket(EXECUTE, 0, NULL);
}

int RTnoProtocol::ReceiveData(unsigned char* packet_buffer) 
{
	char nameBuffer[16];
	int namelen = packet_buffer[DATA_START_ADDR];
	int datalen = packet_buffer[DATA_START_ADDR+1];
	memcpy(nameBuffer, &(packet_buffer[DATA_START_ADDR+2]), namelen);
	nameBuffer[namelen] = 0;

	OutPortWrapperBase* outPort = m_pRTObjectWrapper->GetOutPort(nameBuffer);
	if(outPort == NULL) {
		return -1;
	} 
	
	outPort->Write((void*)(packet_buffer + DATA_START_ADDR + 2 + namelen), datalen / outPort->getTypeSizeInArduino());
	
	return 0;
}

int RTnoProtocol::HandleReceivedPacket(void)
{
	unsigned char packet_buffer[MAX_PACKET_SIZE];
	int ret = m_pTransport->ReceivePacket(packet_buffer);
	if(ret == 0) return 0;
	else if(ret < 0) {
		return -1;
	}

	if(packet_buffer[PACKET_INTERFACE] == RECEIVE_DATA) {
		ReceiveData(packet_buffer);
	}
	
	switch(packet_buffer[PACKET_INTERFACE]) {
	case RECEIVE_DATA:
		ReceiveData(packet_buffer);
		break;
	case EXECUTE:
		if(packet_buffer[DATA_START_ADDR] != RTNO_OK) {
			return -4;
		}
		break;
	case SEND_DATA:
		this->m_SendBusy = FALSE;
		break;
	default:
		std::cout << "Unknown Packet [" << packet_buffer[PACKET_INTERFACE] << "]" << std::endl;
		return -2;
		break;
	}

	return packet_buffer[PACKET_INTERFACE];
}

