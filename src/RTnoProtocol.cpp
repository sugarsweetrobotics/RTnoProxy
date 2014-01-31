#include "stdafx.h"

#include "RTnoRTObjectWrapper.h"
#include "Packet.h"
#include "RTnoProtocol.h"

#include <iostream>


using namespace ssr;
static std::string MSGHDR = "[RTnoProxy] ";

RTnoProtocol::RTnoProtocol(RTnoRTObjectWrapper* pRTObject, Transport *pTransport) :
  m_pTransport(pTransport),m_pRTObjectWrapper(pRTObject)
{
}

RTnoProtocol::~RTnoProtocol(void)
{
}

RTnoPacket RTnoProtocol::waitCommand(const uint8_t command, const uint32_t wait_usec) {
  for(int i = 0;i < 10;i++) {
    if(m_pTransport->isNew()) {
      RTnoPacket pac = m_pTransport->receive(wait_usec);
      if(pac.getInterface() == command) {
	return pac;
      }
    }
  }
  throw TimeOutException();
}

const RTnoProfile&  RTnoProtocol::getRTnoProfile(const uint32_t wait_usec) {
  std::cout << MSGHDR << " - RTnoProtocol::getRTnoProfile() called." << std::endl;
  static const RTnoPacket cmd_packet(GET_PROFILE);
  std::cout << MSGHDR << "    - Transfer Profile Request to Arduino." << std::endl;
  m_pTransport->send(cmd_packet);

  int timeout_count = wait_usec/1000;
  while(1) {
    if(!m_pTransport->isNew()) {
      coil::usleep(1000);
      if (--timeout_count < 0) {
	throw TimeOutException();
      }
      continue;
    }

    RTnoPacket pac = m_pTransport->receive(wait_usec);
    switch(pac.getInterface()) {
    case GET_PROFILE: // Return Code.
      onGetProfile(pac);
      return m_Profile;

    case ADD_INPORT: 
      onAddInPort(pac);
      break;

    case ADD_OUTPORT:
      onAddOutPort(pac);
      break;

    case PACKET_ERROR:
      std::cout << "--RTnoProtocol::getRTnoProfile(): Packet Failed" << std::endl;
      throw GetProfileException();
    default: 
      std::cout << "Unknown Command (" << pac.getInterface() << ")" << std::endl;
    }
  }
}

void RTnoProtocol::onGetProfile(const RTnoPacket& packet)
{
  if (packet.getData()[0] != RTNO_OK) {
    std::cout << "--RTnoProtocol::getRTnoProfile() Failed." << std::endl;
    throw GetProfileException();
  }
}

PortProfile RTnoProtocol::parsePortProfile(const RTnoPacket& packet) {
  char strbuf[64];
  memcpy(strbuf, packet.getData()+1, packet.getDataLength()-1);
  strbuf[packet.getDataLength()-1] = 0;
  return PortProfile(packet.getData()[0], strbuf);
}

void RTnoProtocol::onAddInPort(const RTnoPacket& packet) {
  m_Profile.appendInPort(parsePortProfile(packet));
}

void RTnoProtocol::onAddOutPort(const RTnoPacket& packet) {
  m_Profile.appendOutPort(parsePortProfile(packet));
}

uint8_t RTnoProtocol::getRTnoStatus() {
  static const RTnoPacket cmd_packet(GET_STATUS);
  m_pTransport->send(cmd_packet);
  RTnoPacket pac = waitCommand(GET_STATUS, 20*1000);
  return pac.getData()[0];
}

uint8_t RTnoProtocol::getRTnoExecutionContextType() {
  static const RTnoPacket cmd_packet(GET_CONTEXT);
  m_pTransport->send(cmd_packet);
  RTnoPacket pac = waitCommand(GET_CONTEXT, 20*1000);
  return pac.getData()[0];
}

uint8_t RTnoProtocol::activate() {
  static const RTnoPacket cmd_packet(RTNO_ACTIVATE);
  m_pTransport->send(cmd_packet);
  RTnoPacket pac = waitCommand(RTNO_ACTIVATE, 20*1000);
  return pac.getData()[0];
}

uint8_t RTnoProtocol::reset() {
  static const RTnoPacket cmd_packet(RTNO_RESET);
  m_pTransport->send(cmd_packet);
  RTnoPacket pac = waitCommand(RTNO_RESET, 20*1000);
  return pac.getData()[0];
}

uint8_t RTnoProtocol::deactivate() {
  static const RTnoPacket cmd_packet(RTNO_DEACTIVATE);
  m_pTransport->send(cmd_packet);
  RTnoPacket pac = waitCommand(RTNO_DEACTIVATE, 20*1000);
  return pac.getData()[0];
}

uint8_t RTnoProtocol::sendData(const std::string& portName, const uint8_t* data, const uint8_t length) {
  int namelen = portName.length();//strlen(portName);
  uint8_t buffer[64];
  buffer[0] = namelen;
  buffer[1] = length;
  memcpy(buffer+2, portName.c_str(), namelen);
  memcpy(buffer+2+namelen, data, length);
  RTnoPacket packet(SEND_DATA, buffer, 2 + namelen + length);
  m_pTransport->send(packet);
  return 0;
 }


int32_t RTnoProtocol::sendExecuteTrigger(void) {
  static const RTnoPacket cmd_packet(RTNO_EXECUTE);
  return m_pTransport->send(cmd_packet);
}

void RTnoProtocol::receiveData(const uint8_t* data) 
{
  char name_buffer[16];
  memcpy(name_buffer, data+2, data[0]); name_buffer[data[0]] = 0;
  OutPortWrapperBase* outPort = m_pRTObjectWrapper->GetOutPort(name_buffer);
  if(outPort == NULL) {
    throw UnknownOutPortRequestedException();
  } 
	
  outPort->Write((void*)(data + 2 + data[0]), data[1] / outPort->getTypeSizeInArduino());
}

void RTnoProtocol::handleReceivedPacket(const uint32_t wait_usec) {

  if(m_ProxySynchronousExecution) {
    sendExecuteTrigger();
  }

  bool endFlag = false;
  while(!endFlag) {
    if (m_pTransport->isNew()) {
      try {
	RTnoPacket pac = m_pTransport->receive(wait_usec);
	switch(pac.getInterface()) {
	case RECEIVE_DATA:
	  //ReceiveData(packet_buffer);
	  receiveData(pac.getData());
	  break;
	case RTNO_EXECUTE:
	  if(pac.getData()[0] != RTNO_OK) {
	    throw ExecuteFailedException();
	  }
	  endFlag = true;
	  break;
	case SEND_DATA:
	  //this->m_SendBusy = FALSE;
	  break;
	default:
	  std::cout << "Unknown Packet: " << pac.getInterface() << std::endl;
	  endFlag = true;
	  break;
	}
      } catch (TimeOutException& ex) {
	if(!m_ProxySynchronousExecution) {
	  throw ex;
	} else {
	  break;
	}
      }
    }
  }
}

uint8_t RTnoProtocol::initialize()
{
  uint8_t status = getRTnoStatus();
  std::cout << MSGHDR << " - RTno Status=" << (int)status << std::endl;
  int ret;
  switch(status) {
  case STATE_ACTIVE:
    std::cout << MSGHDR << " -- Now Arduino is in ACTIVE STATE. Deactivating..." << std::endl;
    if((ret = deactivate()) != 0) {
      std::cout << MSGHDR << " -- Failed." << std::endl;
      return false;
    }
    std::cout << MSGHDR << " -- OK." << std::endl;
    break;
  case STATE_INACTIVE:
    std::cout << MSGHDR << " -- Now Arduino is in INACTIVE STATE" << std::endl;
    break;
  case STATE_ERROR:
    std::cout << MSGHDR << " -- Now Arduino is in ERROR STATE. Resetting..." << std::endl;
    if((ret = reset()) != 0) {
      std::cout << MSGHDR << " -- Failed." << std::endl;
      return false;
    }
    std::cout << MSGHDR << " -- OK." << std::endl;
    break;
  }
  
  uint8_t contextType = getRTnoExecutionContextType();
  std::cout << MSGHDR <<  "Execution Context Type == " << (int)contextType << std::endl;
  switch(contextType) {
  case ProxySynchronousExecutionContext:
    std::cout << MSGHDR << " - ProxySynchronousExecutionContext is detected." << std::endl;
    std::cout << MSGHDR << " - This Arduino's on_execute is synchronously executed with this PC.\n" << std::endl;
    std::cout << MSGHDR << " - You can change excution ratio by configuring RTnoProxy's one." << std::endl;
    m_ProxySynchronousExecution = true;
    break;
  default:
    std::cout << " - ProxySynchronousExecutionContext is NOT detected." << std::endl;
    std::cout << " - You can change exeecution ratio by implementing Arduino Code\n" << std::endl;
    m_ProxySynchronousExecution = false;
  }
  

  return 0;
}
