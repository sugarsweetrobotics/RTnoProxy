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
#include "EtherTcp.h"
#include "Serial.h"




// Module specification
// <rtc-template block="module_spec">



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
	//std::cout << "conf.default.comport" << ":" << m_comport << std::endl;
	//std::cout << "conf.default.baudrate" << ":" << m_baudrate << std::endl;
	// Registration: InPort/OutPort/Service
	// <rtc-template block="registration">
	// Set InPort buffers


	// Set OutPort buffer

	// Set service provider to Ports

	// Set service consumers to Ports

	// Set CORBA Service Ports

	// </rtc-template>

	if(m_connectionType == "serial") {
		try {
			std::cout << "Opening SerialPort(" << m_comport << ")....." << std::ends;
			m_pSerialDevice = new ssr::Serial(m_comport.c_str(), m_baudrate);
		} catch (net::ysuga::ComOpenException & e) {
			std::cout << "Fail(" << e.what() << ")" << std::endl;
			return RTC::RTC_ERROR;
		}
	} else if(m_connectionType == "tcp") {
		try {
			std::cout << "Connecting to " << m_ipAddress << ":" << m_portNumber << " with TCP" << std::ends;
			m_pSerialDevice = new ssr::EtherTcp(m_ipAddress.c_str(), m_portNumber);

		} catch (...) {
			std::cout << "Fail" << std::endl;
			return RTC::RTC_ERROR;
		}

	}
	
	m_pRTno = new ssr::RTnoBase(this, m_pSerialDevice);
	
	std::cout << "OK." << std::endl;


	coil::TimeValue interval(0, 1000*1000);
	std::cout << "Waiting for Startup the arduino...\n";
	std::cout << "3......" << std::endl;  coil::sleep(interval);
	std::cout << "2...."   << std::endl;  coil::sleep(interval);
	std::cout << "1.."     << std::endl;  coil::sleep(interval);
	std::cout << "Go!"     << std::endl;

	std::cout << "Starting up onInitialize sequence." << std::endl;
	
	if (!m_pRTno->initialize()) {
	  return RTC::RTC_ERROR;
	}

	return RTC::RTC_OK;
}

RTC::ReturnCode_t RTnoProxy::onFinalize()
{
  delete m_pRTno;
  m_pRTno = NULL;

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
  if(!m_pRTno->activate()) {
    return RTC::RTC_ERROR;
  }

  return RTC::RTC_OK;
}


RTC::ReturnCode_t RTnoProxy::onDeactivated(RTC::UniqueId ec_id)
{
  if(!m_pRTno->deactivate()) {
    return RTC::RTC_ERROR;
  }

  return RTC::RTC_OK;
}


RTC::ReturnCode_t RTnoProxy::onExecute(RTC::UniqueId ec_id)
{
  if(!m_pRTno->execute()) {
    return RTC::RTC_ERROR;
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
  if(!m_pRTno->reset()) {
    return RTC::RTC_ERROR;
  }
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


