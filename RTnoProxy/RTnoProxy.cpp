// -*- C++ -*-
/*!
 * @file  RTnoProxy.cpp
 * @brief Proxy RTC with arduino compatible device and RTnoProxy library
 * @date 11/11/2010
 * @author Yuki Suga (ysuga@ysuga.net)
 * $Id$
 */

#include "stdafx.h"
#include "RTnoProxy.h"
#include "InPortWrapper.h"
#include <coil/Time.h>

#include "Packet.h"

#include "EtherTcp.h"
#include "EtherUdp.h"
#include "Serial.h"


// Module specification
// <rtc-template block="module_spec">

using namespace net::ysuga;

static const char* rtno_spec[] =
  {
    "implementation_id", "RTnoProxy",
    "type_name",         "RTnoProxy",
    "description",       "Proxy RTC with arduino compatible device and RTnoProxy library",
    "version",           "1.0.0",
    "vendor",            "ysuga.net",
    "category",          "Test",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "0",
    "language",          "C++",
    "lang_type",         "compile",
#ifdef WIN32
	//"conf.default.comport", "\\\\.\\COM9",
#else
    //"conf.default.comport", "/dev/tty2",
#endif
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
RTnoProxy::RTnoProxy(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager)
  //    m_doubleInIn("in", m_doubleIn), m_doubleOutOut("out", m_doubleOut)
    // </rtc-template>
{

}

/*!
 * @brief destructor
 */
RTnoProxy::~RTnoProxy()
{
}


RTC::ReturnCode_t RTnoProxy::onInitialize()
{
  bindParameter("connectionType", m_connectionType, "serial");
  bindParameter("ipAddress", m_ipAddress, "192.168.1.2");
  bindParameter("portNumber", m_portNumber, "23");
  bindParameter("comport", m_comport, "/dev/tty0");
  bindParameter("baudrate", m_baudrate, "19200");
  updateParameters("default");
  
  if(m_connectionType == "serial") {
    try {
      std::cout << "Opening SerialPort(" << m_comport << ")....." << std::ends;
      m_pSerialDevice = new Serial(m_comport.c_str(), m_baudrate);
    } catch (ComOpenException & e) {
      std::cout << "Fail(" << e.what() << ")" << std::endl;
      return RTC::RTC_ERROR;
    }
  } else if(m_connectionType == "tcp") {
    try {
      std::cout << "Connecting to " << m_ipAddress << ":" << m_portNumber << " with TCP" << std::ends;
      m_pSerialDevice = new EtherTcp(m_ipAddress.c_str(), m_portNumber);
    } catch (...) {
      std::cout << "Fail" << std::endl;
      return RTC::RTC_ERROR;
    }
    
  } else if(m_connectionType == "udp") {
    try {
      std::cout << "Connecting to " << m_ipAddress << ":" << m_portNumber << " with UDP" << std::ends;
      m_pSerialDevice = new EtherUdp(m_ipAddress.c_str(), m_portNumber);
    } catch (...) {
      std::cout << "Failed." << std::endl;
      return RTC::RTC_ERROR;
    }
  }
  m_pTransport = new Transport(m_pSerialDevice);
  m_pRTObjectWrapper = new RTnoRTObjectWrapper(this);
  m_pProtocol = new RTnoProtocol(m_pRTObjectWrapper, m_pTransport);
  
  std::cout << "OK." << std::endl;
  
  
  coil::TimeValue interval(0, 1000*1000);
  std::cout << "Waiting for Startup the arduino...\n";
  std::cout << "3......" << std::endl;  coil::sleep(interval);
  std::cout << "2...."   << std::endl;  coil::sleep(interval);
  std::cout << "1.."     << std::endl;  coil::sleep(interval);
  std::cout << "Go!"     << std::endl;
  
  std::cout << "Starting up onInitialize sequence." << std::endl;
  
  m_pProtocol->GetRTnoProfile(&m_Profile); 
  PortList* pInPortList = m_Profile.GetInPortList();
  int i = 0;
  char target[4] = {1, 1, 1, 1,};
  for(PortListIterator it = pInPortList->begin();it != pInPortList->end();++it) {
    m_pRTObjectWrapper->AddInPort((*it));
    m_pProtocol->ConnectInPort("UART", i, target, i);
    i++;
  }
  PortList* pOutPortList = m_Profile.GetOutPortList();
  i = 0;
  for(PortListIterator it = pOutPortList->begin();it != pOutPortList->end();++it) {
    m_pRTObjectWrapper->AddOutPort((*it));
    m_pProtocol->ConnectOutPort("UART", i, target, i);
    i++;
  }
  
  unsigned char status = m_pProtocol->GetRTnoStatus();
  std::cout << "RTno Status == " << (int)status << std::endl;
  int ret;
  switch(status) {
  case RTC_STATE_ACTIVE:
    if((ret = m_pProtocol->DeactivateRTno()) != 0) {
      return RTC::RTC_ERROR;
    }
    break;
  case RTC_STATE_INACTIVE:
    break;
  case RTC_STATE_ERROR:
    if((ret = m_pProtocol->ResetRTno()) != 0) {
      return RTC::RTC_ERROR;
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

RTC::ReturnCode_t RTnoProxy::onFinalize()
{
	delete m_pProtocol;
	delete m_pTransport;
	delete m_pSerialDevice;
	return RTC::RTC_OK;
}


/*
RTC::ReturnCode_t RTnoProxy::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RTnoProxy::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t RTnoProxy::onActivated(RTC::UniqueId ec_id)
{

	int ret;
	if((ret = m_pProtocol->ActivateRTno()) == 0) {
		
		return RTC::RTC_OK;
	}

	std::cout << "onActivated failed." << std::endl;
	std::cout << " -- Error Code is " << ret << std::endl;
	return RTC::RTC_ERROR;	
}


RTC::ReturnCode_t RTnoProxy::onDeactivated(RTC::UniqueId ec_id)
{
	int ret;
	if((ret = m_pProtocol->DeactivateRTno()) == 0) {
		return RTC::RTC_OK;
	}

	std::cout << "onDeactivated failed." << std::endl;
	std::cout << " -- Error Code is " << ret << std::endl;
	return RTC::RTC_ERROR;	
}


RTC::ReturnCode_t RTnoProxy::onExecute(RTC::UniqueId ec_id)
{
	InPortMap* pInPortMap = m_pRTObjectWrapper->GetInPortMap();
	//	uint8_t i = 0;
	//	for(InPortMapIterator it = pInPortMap->begin();it != pInPortMap->end();++it, i++) {
	//		std::string name = (*it).first;
	//		InPortWrapperBase* inPort = (*it).second;
	for(int i = 0;i < m_pRTObjectWrapper->GetNumInPort();i++) {
	  InPortWrapperBase* inPort = m_pRTObjectWrapper->GetInPort(i);
		if(inPort->isNew()) {
		  unsigned char packet_buffer[256];
			int len = inPort->Read();
			inPort->Get(packet_buffer, len);
			m_pProtocol->SendData(i, packet_buffer, len * inPort->getTypeSizeInArduino());
		}
		//	}
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
			return RTC::RTC_ERROR;
		}

		if(ret == CONTROL_EXECUTE && m_ProxySynchronousExecution) {
			break;
		}
	}

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t RTnoProxy::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t RTnoProxy::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RTnoProxy::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t RTnoProxy::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RTnoProxy::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void RTnoProxyInit(RTC::Manager* manager)
  {
    coil::Properties profile(rtno_spec);
    manager->registerFactory(profile,
                             RTC::Create<RTnoProxy>,
                             RTC::Delete<RTnoProxy>);
  }
  
};


