

#include "stdafx.h"
#include <coil/Time.h>

#include "Packet.h"


#include "RTnoRTObjectWrapper.h"
#include "RTnoProtocol.h"
#include "Transport.h"
#include "InPortWrapper.h"
#include "RTnoBase.h"

enum {
  CREATED='C',
  INACTIVE='I',
  ACTIVE='A',
  RTCERROR='E',
  NONE='N',
};

using namespace net::ysuga;
using namespace ssr;

RTnoBase::RTnoBase(RTC::DataFlowComponentBase* pRTC, net::ysuga::SerialDevice* pSerial) {
  m_pRTC = pRTC;
  m_pSerialDevice = pSerial;
  m_pTransport = new Transport(m_pSerialDevice);
  m_pRTObjectWrapper = new RTnoRTObjectWrapper(pRTC);
  m_pProtocol = new RTnoProtocol(m_pRTObjectWrapper, m_pTransport);
}


RTnoBase::~RTnoBase()
{
  delete m_pProtocol;
  delete m_pRTObjectWrapper;
  delete m_pTransport;
}



bool RTnoBase::initialize()
{
  m_pProtocol->GetRTnoProfile(&m_Profile); 
  PortList* pInPortList = m_Profile.GetInPortList();
  for(PortListIterator it = pInPortList->begin();it != pInPortList->end();++it) {
    m_pRTObjectWrapper->AddInPort((*it));
  }
  PortList* pOutPortList = m_Profile.GetOutPortList();
  for(PortListIterator it = pOutPortList->begin();it != pOutPortList->end();++it) {
    m_pRTObjectWrapper->AddOutPort((*it));
  }
  
  unsigned char status = m_pProtocol->GetRTnoStatus();
  std::cout << "RTno Status == " << (int)status << std::endl;
  int ret;
  switch(status) {
  case ACTIVE:
    if((ret = m_pProtocol->DeactivateRTno()) != 0) {
      return false;
    }
    break;
  case INACTIVE:
    break;
  case RTCERROR:
    if((ret = m_pProtocol->ResetRTno()) != 0) {
      return false;
    }
    break;
  }
  
  unsigned char contextType = m_pProtocol->GetRTnoExecutionContextType();
  std::cout << "Execution Context Type == " << (int)contextType << std::endl;
  switch(contextType) {
  case ProxySynchronousExecutionContext:
    std::cout << "--ProxySynchronousExecutionContext detected!" << std::endl;
    this->m_ProxySynchronousExecution = true;
    break;
  default:
    m_ProxySynchronousExecution = false;
  }
  
  std::cout << "onInitialized OK." << std::endl;
  return RTC::RTC_OK;
}



bool RTnoBase::activate()
{
  int ret;
  if((ret = m_pProtocol->ActivateRTno()) == 0) {
    
    return true;
  }
  
  std::cout << "onActivated failed." << std::endl;
  std::cout << " -- Error Code is " << ret << std::endl;
  return false;
}


bool RTnoBase::deactivate()
{
  int ret;
  if((ret = m_pProtocol->DeactivateRTno()) == 0) {
    return true;
  }
  
  std::cout << "onDeactivated failed." << std::endl;
  std::cout << " -- Error Code is " << ret << std::endl;
  return false;
}


bool RTnoBase::execute()
{
  InPortMap* pInPortMap = m_pRTObjectWrapper->GetInPortMap();
  for(InPortMapIterator it = pInPortMap->begin();it != pInPortMap->end();++it) {
    std::string name = (*it).first;
    InPortWrapperBase* inPort = (*it).second;
    if(inPort->isNew()) {
      unsigned char packet_buffer[MAX_PACKET_SIZE];
      int len = inPort->Read();
      inPort->Get(packet_buffer, len);
      m_pProtocol->SendData(name.c_str(), packet_buffer, len * inPort->getTypeSizeInArduino());
    }
  }
  
  if(this->m_ProxySynchronousExecution) {
    m_pProtocol->SendExecuteTrigger();
  }
  
  
  while(1) {
    int ret = m_pProtocol->HandleReceivedPacket();
    if(ret == 0) { // No Data Received.
      if(m_ProxySynchronousExecution) {
	continue; // if ProxySynchronousExecution, wait for EXECUTE packet.
      }
      break;
    }
    
    if(ret < 0) {
      std::cout << "--RTnoProxy::OnExecute() failed (error code = " << ret << ")" << std::endl;
      return false;
    }
    
    if(ret == EXECUTE && m_ProxySynchronousExecution) {
      break;
    }
  }
  
  return true;
}



bool RTnoBase::reset()
{
  return true;
}
