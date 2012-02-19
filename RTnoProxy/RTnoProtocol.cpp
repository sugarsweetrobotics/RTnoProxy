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

std::string RTnoProtocol::GetStringFromPacket(const int8_t* start_adr, int length)
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
  
  SendRequest(GET_PROFILE);
  //  PacketBuffer command(GET_PROFILE, (int8_t*)"UART", 0xCC, 0xCC, NULL, 0);
  //  m_pTransport->SendPacket("UART", &command);
  while(1) {

    PacketBuffer* packet = m_pTransport->ReceivePacket();
    if(packet == NULL) continue;
    
    std::cout << "---packet received." << std::endl;
    std::string strbuf;
    int8_t buf;
    int8_t* pDataBuffer = packet->getDataBuffer();
    switch(packet->getInterface()) {
    case GET_PROFILE: // Return Code.
      buf = pDataBuffer[0];
      if(buf == RTNO_OK) {
	std::cout << "--RTnoProtocol::getRTnoProfile() Succeeded." << std::endl;
	return 0;
      } else {
	std::cout << "--RTnoProtocol::getRTnoProfile() Failed." << std::endl;
	return -2;
      }
      break;
    case ADD_INPORT: 
      strbuf = GetStringFromPacket(pDataBuffer + 1, packet->getDataLength()-1);
      profile->AddInPort(pDataBuffer[0], strbuf.c_str());
      break;
    case ADD_OUTPORT:
      strbuf = GetStringFromPacket(pDataBuffer + 1, packet->getDataLength()-1);
      profile->AddOutPort(pDataBuffer[0], strbuf.c_str());
      break;
    case PACKET_ERROR:
      std::cout << "Packet Error (F): return value -> " << (int)(pDataBuffer[0]) << std::endl;
      ///      delete packet;
      return -1;
      
    default:
      std::cout << "Unknown Command (" << packet->getInterface() << ")" << std::endl;
      //delete packet;
      return -1;
    }
    //    delete packet;
    
  }
}

void RTnoProtocol::SendRequest(int8_t interface) {
  PacketBuffer packet(interface, (int8_t*)"UART", 0xCC, 0xCC, NULL, 0);
  m_pTransport->SendPacket("UART", &packet);
}


unsigned char RTnoProtocol::GetRTnoStatus()
{	
	std::cout << "-RTnoProtocol::GetRTnoStatus() called." << std::endl;
	
	SendRequest(GET_STATUS);
	return ReceiveReturnCode(GET_STATUS);
}

unsigned char RTnoProtocol::GetRTnoExecutionContextType()
{
	std::cout << "-RTnoProtocol::GetRTnoExecutionContextType() called." << std::endl;
	//	unsigned char packet_buffer[MAX_PACKET_SIZE];
	
	///m_pTransport->SendPacket(GET_CONTEXT, 0, NULL);
	SendRequest(GET_CONTEXT);
	return ReceiveReturnCode(GET_CONTEXT);
}

int RTnoProtocol::ReceiveReturnCode(unsigned char intf) {
  //  unsigned char packet_buffer[MAX_PACKET_SIZE];

  while(1) {
    PacketBuffer* packet = m_pTransport->ReceivePacket();
    //	  int ret = m_pTransport->ReceivePacket(packet_buffer);
    if(packet == NULL) continue;
    
    //		if(ret == 0) continue;
    
    //		if(ret == -TIMEOUT) {
    //			std::cout << "---Timeout Error." << std::endl;
    //			return -1; // Timeout or Checksum error.
    //		} else if(ret == -CHECKSUM_ERROR) {
    //			std::cout << "---Checksum Error." << std::endl;
    //			return -1;
    //	      }
    
    if(packet->getInterface() == intf) {
      return *(packet->getDataBuffer());
    }
    
    switch(packet->getInterface()) {
    case SEND_DATA:
      m_SendBusy = FALSE;
      continue;
    case RECEIVE_DATA: {
      //	    ReceiveData(packet_buffer);
      OutPortWrapperBase* outPort = m_pRTObjectWrapper->GetOutPort(packet->getSourcePortIndex());
      if(outPort == NULL) {
	return -1;
      }
      outPort->Write(packet->getDataBuffer(), packet->getDataLength() / outPort->getTypeSizeInArduino());
      continue;
    }
    default:
      //      std::cout << "---Unknown Packet Interface [" << packet_buffer[PACKET_INTERFACE] << "] expected:" << intf << std::endl;
      std::cout << "--Unknown Packet Interface[" << packet->getInterface() << "]";
      return -2;
    }
  }
}


int RTnoProtocol::ActivateRTno()
{
  std::cout << "-RTnoProtocol::ActivateRTno() called...." << std::ends;
  //m_pTransport->SendPacket(ACTIVATE, 0, NULL);
  SendRequest(ACTIVATE);
  int ret = ReceiveReturnCode(ACTIVATE);
  if(ret != RTNO_OK) {
    std::cout << "Failed (ret = " << ret << ")" << std::endl;
    return ret;
  }
  std::cout << "Succeed." << std::endl;
  return 0;
}

int RTnoProtocol::ResetRTno() 
{
  std::cout << "-RTnoProtocol::ErrorRTno() called....";
  //  m_pTransport->SendPacket(RTNO_RESET, 0, NULL);
  SendRequest(RTNO_RESET);
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
  ///  /m_pTransport->SendPacket(DEACTIVATE, 0, NULL);
  SendRequest(DEACTIVATE);
  int ret = ReceiveReturnCode(DEACTIVATE);
  if(ret != RTNO_OK) {
    std::cout << "Failed." << std::endl;
    return ret;
  }
  std::cout << "Succeed." << std::endl;
  return 0;
}


int RTnoProtocol::SendData(const uint8_t index, const unsigned char* data, int length)
{
	while(this->m_SendBusy) {
		coil::TimeValue tv(1000.0); // 1 ms;
		coil::sleep(tv);
	}
	//std::cout << "-RTnoProtocol::SendData() called."<< std::endl;
	//unsigned char packet_buffer[MAX_PACKET_SIZE];

	//packet_buffer[0] = index;//namelen;
	//	packet_buffer[1] = length;
	//	memcpy(packet_buffer+2, data, length);
	//	m_pTransport->SendPacket(SEND_DATA, length + 2, packet_buffer);
	PacketBuffer packet(SEND_DATA, (int8_t*)"UART", 0xCC, index,(int8_t*)data, length);
	m_pTransport->SendPacket("UART", &packet);

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
  //return m_pTransport->SendPacket(EXECUTE, 0, NULL);
  SendRequest(EXECUTE);
  return 0;
}

//int RTnoProtocol::ReceiveData(unsigned char* packet_buffer) 
int RTnoProtocol::ReceiveData(PacketBuffer* packet)
{
  //char nameBuffer[16];
	//	int namelen = packet_buffer[DATA_START_ADDR];
  //	int index = packet_buffer[DATA_START_ADDR];
  int index = packet->getSourcePortIndex();
  //	int datalen = packet_buffer[DATA_START_ADDR+1];
  //  int packet->getDataLength();
	//memcpy(nameBuffer, &(packet_buffer[DATA_START_ADDR+2]), namelen);
	//	nameBuffer[namelen] = 0;

	//	OutPortWrapperBase* outPort = m_pRTObjectWrapper->GetOutPort(nameBuffer);
  OutPortWrapperBase* outPort = m_pRTObjectWrapper->GetOutPort(index);
  if(outPort == NULL) {
    return -1;
  } 
	
  //	outPort->Write((void*)(packet_buffer + DATA_START_ADDR + 2 + namelen), datalen / outPort->getTypeSizeInArduino());
  ///  outPort->Write((void*)(packet_buffer + DATA_START_ADDR + 2), datalen / outPort->getTypeSizeInArduino());
  outPort->Write((void*)packet->getDataBuffer(), packet->getDataLength() / outPort->getTypeSizeInArduino());
	
	return 0;
}

int RTnoProtocol::HandleReceivedPacket(void)
{
  //unsigned char packet_buffer[MAX_PACKET_SIZE];
  //int ret = m_pTransport->ReceivePacket(packet_buffer);
  PacketBuffer* packet = m_pTransport->ReceivePacket();
  if(packet == NULL) return 0;
  
  switch(packet->getInterface()) {
  case RECEIVE_DATA:
    ReceiveData(packet);
    break;
    
  case EXECUTE:
    if(*(packet->getDataBuffer()) != RTNO_OK) {
      return -4;
    }
    break;
  case SEND_DATA:
    this->m_SendBusy = FALSE;
    break;
  default:
    //    std::cout << "Unknown Packet [" << packet_buffer[PACKET_INTERFACE] << "]" << std::endl;
    std::cout << "Unknown Packet[" << packet->getInterface() << "]" << std::endl;
    return -2;
    break;
  }
  
  return packet->getInterface();
}

