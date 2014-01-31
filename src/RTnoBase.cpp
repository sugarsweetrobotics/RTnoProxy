#include "stdafx.h"
#include <coil/Time.h>

#include "Packet.h"


#include "RTnoRTObjectWrapper.h"
#include "RTnoProtocol.h"
#include "Transport.h"
#include "InPortWrapper.h"
#include "RTnoBase.h"


//using namespace net::ysuga;
using namespace ssr;

RTnoBase::RTnoBase(RTC::DataFlowComponentBase* pRTC, SerialDevice* pSerial) {
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

static std::string MSGHDR = "[RTnoProxy] ";

bool RTnoBase::initialize()
{
  try {
    RTnoProfile profile = m_pProtocol->getRTnoProfile(10*1000*1000);

  std::cout << MSGHDR << " - Parsing RTnoProfile." << std::endl;
  PortList inPorts = profile.inPorts();
  for(PortListIterator it = inPorts.begin();it != inPorts.end();++it) {
    std::cout << MSGHDR << " -- Adding InPort  (name=" << (*it).getPortName()
	      << ", typeCode=" << (*it).getTypeCode() << ")\n";
    m_pRTObjectWrapper->addInPort((*it));
  }
  PortList outPorts = profile.outPorts();
  for(PortListIterator it = outPorts.begin();it != outPorts.end();++it) {
    std::cout << MSGHDR << " -- Adding OutPort (name=" << (*it).getPortName()
	      << ", typeCode=" << (*it).getTypeCode() << ")\n";

    m_pRTObjectWrapper->addOutPort((*it));
  }
  std::cout << MSGHDR << " - Success." << std::endl;
  m_pProtocol->initialize();
  std::cout << MSGHDR << " - onInitialized OK." << std::endl;
  return true;
  } catch (TimeOutException &e) {
    std::cout << MSGHDR << " - TimeOutException" << std::endl;
    return false;
  }

}



bool RTnoBase::activate()
{
  m_pProtocol->activate();
  return true;
}


bool RTnoBase::deactivate()
{
  m_pProtocol->deactivate();
  return true;
}


bool RTnoBase::execute()
{
  InPortMap* pInPortMap = m_pRTObjectWrapper->GetInPortMap();
  for(InPortMapIterator it = pInPortMap->begin();it != pInPortMap->end();++it) {
    std::string name = (*it).first;
    InPortWrapperBase* inPort = (*it).second;

    if(inPort->isNew()) {
      std::cout << "-reading InPort:" << name << std::endl;
      std::cout << "--received." << std::endl;
      unsigned char packet_buffer[MAX_PACKET_SIZE];
      int len = inPort->Read();
      inPort->Get(packet_buffer, len);
      m_pProtocol->sendData(name.c_str(), packet_buffer, len * inPort->getTypeSizeInArduino());
    }
  }


  
  m_pProtocol->handleReceivedPacket(RTNO_INFINITE);
  
  return true;
}



bool RTnoBase::reset()
{
  return true;
}
