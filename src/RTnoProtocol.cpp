#include "stdafx.h"

#include "RTnoRTObjectWrapper.h"
#include "Packet.h"
#include "RTnoProtocol.h"

#include <iostream>


using namespace ssr;

RTnoProtocol::RTnoProtocol(RTnoRTObjectWrapper* pRTObject, Transport *pTransport) :
  m_SendBusy(false), m_pRTObjectWrapper(pRTObject), m_pTransport(pTransport)
{
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

bool RTnoProtocol::waitCommand(const uint8_t command, const uint32_t wait_usec)
{
  RTnoPacket cmd_packet(command);
  for(int i = 0;i < 10;i++) {
    if(m_pTransport->isNew()) {
      RTnoPacket pac = m_pTransport->receive(wait_usec);
      if(pac.getInterface() == command) {
	return true;
      }
    }
  }
  return false;
}

RTnoPacket  RTnoProtocol::getRTnoProfile() {
  std::cout << "-RTnoProtocol::getRTnoProfile() called." << std::endl;
  RTnoProfile prof;
  const RTnoPacket cmd_packet(GET_PROFILE);
  m_pTransport->send(cmd_packet);
  while(1) {
    if(!m_pTransport->isNew()) {
      coil::usleep(1000);
      continue;
    }

    RTnoPacket pac = m_pTransport->receive();
    switch(pac.getInterface()) {
    case GET_PROFILE: // Return Code.
      if(pac.getData()[0] == RTNO_OK) {
	std::cout << "--RTnoProtocol::getRTnoProfile() Succeeded." << std::endl;
	return prof;
      } else {
	std::cout << "--RTnoProtocol::getRTnoProfile() Failed." << std::endl;
	throw GetProfileException();
      }
      break;
    case ADD_INPORT: 
      
      strbuf = GetStringFromPacket(packet_buffer+DATA_START_ADDR+1, packet_buffer[DATA_LENGTH]-1);
      std::cout << "--RTnoProtocol::AddInPort(" << strbuf << ":type_code=" << packet_buffer[DATA_START_ADDR] << std::endl;
      profile->AddInPort(packet_buffer[DATA_START_ADDR], strbuf.c_str());
      break;
    case ADD_OUTPORT:
      strbuf = GetStringFromPacket(packet_buffer+DATA_START_ADDR+1, packet_buffer[DATA_LENGTH]-1);
      std::cout << "--RTnoProtocol::AddOutPort(" << strbuf << ":type_code=" << packet_buffer[DATA_START_ADDR] << std::endl;
      profile->AddOutPort(packet_buffer[DATA_START_ADDR], strbuf.c_str());
      break;
    case PACKET_ERROR:
      std::cout << "--RTnoProtocol::getRTnoProfile(): Packet Failed :retval = " << (int)((uint8_t*)packet_buffer)[DATA_LENGTH+1] << std::endl;
      return (int)((int8_t*)packet_buffer)[DATA_LENGTH+1];
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
	  if (m_SendBusy) {
	    // if timeout, m_SendBusy = false;
	  }

		int ret = m_pTransport->ReceivePacket(packet_buffer);
		if(ret == 0) continue;
		if(ret == -TIMEOUT) {
			std::cout << "---Timeout Error." << std::endl;
			return -TIMEOUT; // Timeout or Checksum error.
		} else if(ret == -CHECKSUM_ERROR) {
			std::cout << "---Checksum Error." << std::endl;
			return -CHECKSUM_ERROR;
		}

		if(packet_buffer[PACKET_INTERFACE] == SEND_DATA) {
			m_SendBusy = FALSE;
			continue;
		}

		if(packet_buffer[PACKET_INTERFACE] == RECEIVE_DATA) {
			ReceiveData(packet_buffer);
			continue;
		}

		if(packet_buffer[PACKET_INTERFACE] == PACKET_ERROR) {
		  std::cout << "--RTnoProtocol::GetReturnCode : Packet Error :ret = " << (int)((uint8_t*)packet_buffer)[DATA_LENGTH+1] << std::endl;
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
  std::cout << "-RTnoProtocol::ActivateRTno() called...." << std::ends;
  m_pTransport->SendPacket(ACTIVATE, 0, NULL);
  int ret = ReceiveReturnCode(ACTIVATE);
  if(ret != RTNO_OK) {
    std::cout << "Failed." << std::endl;
    return ret;
  }
  std::cout << "Succeed." << std::endl;
  return 0;
}

int RTnoProtocol::ResetRTno() 
{
  std::cout << "-RTnoProtocol::ErrorRTno() called....";
  m_pTransport->SendPacket(RTNO_RESET, 0, NULL);
  int ret = ReceiveReturnCode(RTNO_RESET);
  if(ret != RTNO_OK) {
    std::cout << "Failed." << std::endl;
    return ret;
  }
  std::cout << "Success." << std::endl;
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
	std::cout << "-RTnoProtocol::SendData(" << portName << ") called."<< std::endl;
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
	int ret = m_pTransport->ReceivePacket(packet_buffer, 10*1000);
	if(ret == 0) return 0;
	else if(ret < 0) {
	  if (ret == -CHECKSUM_ERROR) {
	    //return 0;
	  }
	  std::cout << "-RTnoProtocol::HandleReceivedPacket() failed: ret-" << ret << std::endl;
	  return ret;
	}
	
	//if(packet_buffer[PACKET_INTERFACE] == RECEIVE_DATA) {
	//	ReceiveData(packet_buffer);
	//}
	
	//std::cout << "--RTnoProtocol::HandleReceivePacket() received:" << packet_buffer[PACKET_INTERFACE] << std::endl;

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
	  std::cout << "SEND_DATA return code received: ret=" << (int)packet_buffer[DATA_START_ADDR] << std::endl;
		this->m_SendBusy = FALSE;
		break;
	default:
		std::cout << "Unknown Packet [" << packet_buffer[PACKET_INTERFACE] << "]" << std::endl;
		return -2;
		break;
	}

	return packet_buffer[PACKET_INTERFACE];
}

