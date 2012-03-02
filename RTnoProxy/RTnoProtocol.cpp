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
  
  SendRequest(CONTROL_GET_PROFILE);
  while(1) {
    PacketBuffer* packet = m_pTransport->ReceivePacket();
    if(packet == NULL) continue;
    
    char intf = packet->getInterface();
    std::cout << "---packet received(interface=" << intf << std::endl;
    std::string strbuf;
    int8_t buf;
    int8_t* pDataBuffer = packet->getDataBuffer();
    switch(packet->getInterface()) {
    case CONTROL_GET_PROFILE: // Return Code.
      buf = pDataBuffer[0];
      if(buf == RTNO_RTC_OK) {
	std::cout << "--RTnoProtocol::getRTnoProfile() Succeeded." << std::endl;
	return 0;
      } else {
	std::cout << "--RTnoProtocol::getRTnoProfile() Failed." << std::endl;
	return -2;
      }
      break;
    case PROFILE_INPORT: 
      strbuf = GetStringFromPacket(pDataBuffer + 1, packet->getDataLength()-1);
      profile->AddInPort(pDataBuffer[0], strbuf.c_str());
      break;
    case PROFILE_OUTPORT:
      strbuf = GetStringFromPacket(pDataBuffer + 1, packet->getDataLength()-1);
      profile->AddOutPort(pDataBuffer[0], strbuf.c_str());
      break;
    case PACKET_ERROR:
      std::cout << "Packet Error (F): return value -> " << (int)(pDataBuffer[0]) << std::endl;
      return -1;
    default:
      std::cout << "Unknown Command (" << packet->getInterface() << ")" << std::endl;
      return -1;
    }
  }
}

void RTnoProtocol::SendRequest(int8_t interface) {
  PacketBuffer packet(interface, (int8_t*)"UART", CONTROL_PORT_INDEX, CONTROL_PORT_INDEX, NULL, 0);
  m_pTransport->SendPacket((char*)"UART", &packet);
}

unsigned char RTnoProtocol::GetRTnoStatus()
{	
	std::cout << "-RTnoProtocol::GetRTnoStatus() called." << std::endl;
	SendRequest(CONTROL_GET_STATUS);
	return ReceiveReturnCode(CONTROL_GET_STATUS);
}

unsigned char RTnoProtocol::GetRTnoExecutionContextType() 
{
  std::cout << "-RTnoProtocol::GetRTnoExecutionContextType() called." << std::endl;
  SendRequest(CONTROL_GET_CONTEXT);
  return ReceiveReturnCode(CONTROL_GET_CONTEXT);
}

int RTnoProtocol::ReceiveReturnCode(unsigned char intf) {
  while(1) {
    PacketBuffer* packet = m_pTransport->ReceivePacket();
    if(packet == NULL) continue;
    
    if(packet->getInterface() == intf) {
      return *(packet->getDataBuffer());
    }
    
    switch(packet->getInterface()) {
    case CONTROL_INPORT_CONNECT:
      if(*(packet->getDataBuffer()) != RTNO_RTC_OK) {
	std::cout << "ConnectInPort Failed(" <<*(packet->getDataBuffer()) <<")" << std::endl;
      } else {
	std::cout << "ConnectInPort OK." << std::endl;
      }
      break;
    case CONTROL_OUTPORT_CONNECT:
      if(*(packet->getDataBuffer()) != RTNO_RTC_OK) {
	std::cout << "ConnectOutPort Failed(" <<*(packet->getDataBuffer()) <<")" << std::endl;
      } else {
	std::cout << "ConnectOutPort OK." << std::endl;
      }
      break;
    case DATAPORT_DATA: {
      OutPortWrapperBase* outPort = m_pRTObjectWrapper->GetOutPort(packet->getSourcePortIndex());
      if(outPort == NULL) {
	return -1;
      }
      outPort->Write(packet->getDataBuffer(), packet->getDataLength() / outPort->getTypeSizeInArduino());
      continue;
    }
    default:
      std::cout << "--Unknown Packet Interface[" << packet->getInterface() << "]";
      return -2;
    }
  }
}


int RTnoProtocol::ActivateRTno()
{
  std::cout << "-RTnoProtocol::ActivateRTno() called...." << std::ends;
  SendRequest(CONTROL_ACTIVATE);
  int ret = ReceiveReturnCode(CONTROL_ACTIVATE);
  if(ret != RTNO_RTC_OK) {
    std::cout << "Failed (ret = " << ret << ")" << std::endl;
    return ret;
  }
  std::cout << "Succeed." << std::endl;
  return 0;
}

int RTnoProtocol::ResetRTno() 
{
  std::cout << "-RTnoProtocol::ErrorRTno() called....";
  SendRequest(CONTROL_RESET);
  int ret = ReceiveReturnCode(CONTROL_RESET);
  if(ret != RTNO_RTC_OK) {
    std::cout << "Failed." << std::endl;
    return ret;
  }
  std::cout << "Success." << std::endl;
  return 0;
}


int RTnoProtocol::DeactivateRTno()
{
  std::cout << "-RTnoProtocol::DeactivateRTno() called....";
  SendRequest(CONTROL_DEACTIVATE);
  int ret = ReceiveReturnCode(CONTROL_DEACTIVATE);
  if(ret != RTNO_RTC_OK) {
    std::cout << "Failed." << std::endl;
    return ret;
  }
  std::cout << "Succeed." << std::endl;
  return 0;
}

int RTnoProtocol::ConnectInPort(const char* sourceAddr, const uint8_t sourcePort, const char* targetAddress, const uint8_t targetPort)
{
  std::cout << "-RTnoProtocol::ConnectInPort() called..." << std::endl;
  int8_t buffer[2 + PKT_SOURCE_ADDR_SIZE];
  buffer[0] = sourcePort;
  memcpy(buffer+1, targetAddress, PKT_SOURCE_ADDR_SIZE);
  buffer[1+PKT_SOURCE_ADDR_SIZE] = targetPort;
  
  PacketBuffer packet(CONTROL_INPORT_CONNECT, (int8_t*)"UART", CONTROL_PORT_INDEX, CONTROL_PORT_INDEX, buffer, 2+PKT_SOURCE_ADDR_SIZE);
  m_pTransport->SendPacket((char*)sourceAddr, &packet);
  return 0;
}

int RTnoProtocol::ConnectOutPort(const char* sourceAddr, const uint8_t sourcePort, const char* targetAddress, const uint8_t targetPort)

{
  std::cout << "-RTnoProtocol::ConnectOutPort() called..." << std::endl;
  int8_t buffer[2 + PKT_SOURCE_ADDR_SIZE];
  buffer[0] = sourcePort;
  memcpy(buffer+1, targetAddress, PKT_SOURCE_ADDR_SIZE);
  buffer[1+PKT_SOURCE_ADDR_SIZE] = targetPort;
  PacketBuffer packet(CONTROL_OUTPORT_CONNECT, (int8_t*)"UART", CONTROL_PORT_INDEX, CONTROL_PORT_INDEX, buffer, 2+PKT_SOURCE_ADDR_SIZE);
  m_pTransport->SendPacket((char*)sourceAddr, &packet);
  return 0;
}

int RTnoProtocol::SendData(const uint8_t index, const unsigned char* data, int length)
{
  while(this->m_SendBusy) {
    coil::TimeValue tv(1000.0); // 1 ms;
    coil::sleep(tv);
  }
  PacketBuffer packet(DATAPORT_DATA, (int8_t*)"UART", index, index, (int8_t*)data, length);
  m_pTransport->SendPacket((char*)"UART", &packet);
  return 0;
}


int RTnoProtocol::SendExecuteTrigger(void)
{
  SendRequest(CONTROL_EXECUTE);
  return 0;
}

int RTnoProtocol::ReceiveData(PacketBuffer* packet)
{
  int index = packet->getSourcePortIndex();
  OutPortWrapperBase* outPort = m_pRTObjectWrapper->GetOutPort(index);
  if(outPort == NULL) {
    return -1;
  } 
  outPort->Write((void*)packet->getDataBuffer(), packet->getDataLength() / outPort->getTypeSizeInArduino());
  return 0;
}

int RTnoProtocol::HandleReceivedPacket(void)
{
  PacketBuffer* packet = m_pTransport->ReceivePacket();
  if(packet == NULL) return 0;
  
  switch(packet->getInterface()) {
  case DATAPORT_DATA:
    ReceiveData(packet);
    break;
  case CONTROL_EXECUTE:
    if(*(packet->getDataBuffer()) != RTNO_RTC_OK) {
      return -4;
    }
    break;
  case CONTROL_INPORT_CONNECT:
    std::cout << "ConnectInPort OK." << std::endl;
    break;
  case CONTROL_OUTPORT_CONNECT:
    std::cout << "ConnectOutPort OK." << std::endl;
    break;
  default:
    std::cout << "Unknown Packet[" << packet->getInterface() << "]" << std::endl;
    return -2;
    break;
  }
  
  return packet->getInterface();
}

