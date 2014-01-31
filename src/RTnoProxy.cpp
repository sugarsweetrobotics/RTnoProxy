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




static std::string MSGHDR = "[RTnoProxy] ";

RTC::ReturnCode_t RTnoProxy::onInitialize()
{
	bindParameter("connectionType", m_connectionType, "serial");
	bindParameter("ipAddress", m_ipAddress, "192.168.1.2");
	bindParameter("portNumber", m_portNumber, "23");
	bindParameter("comport", m_comport, "/dev/tty0");
	bindParameter("baudrate", m_baudrate, "19200");


	updateParameters("default");
	std::cout << MSGHDR << "Configuration Values" << std::endl;
	std::cout << MSGHDR << " - conf.default.connectionType" << ":" << m_connectionType << std::endl;
	if (m_connectionType == "serial") {
	  std::cout << MSGHDR << " - Serial Port Connection" << std::endl;
	  std::cout << MSGHDR << "    - conf.default.comport" << ":" << m_comport << std::endl;
	  std::cout << MSGHDR << "    - conf.default.baudrate" << ":" << m_baudrate << std::endl;
	} else {
	  std::cout << MSGHDR << " - TCP Port Connection" << std::endl;
	  std::cout << MSGHDR << "    - conf.default.portNumber" << ":" << m_portNumber << std::endl;
	  std::cout << MSGHDR << "    - conf.default.ipAddress" << ":" << m_ipAddress << std::endl;
	}
 
	// <rtc-template block="registration">
	// Set InPort buffers


	// Set OutPort buffer

	// Set service provider to Ports

	// Set service consumers to Ports

	// Set CORBA Service Ports

	// </rtc-template>

	if(m_connectionType == "serial") {
	  try {
	    std::cout << MSGHDR << "Opening SerialPort(" << m_comport << ")....." << std::ends;
	    m_pSerialDevice = new ssr::Serial(m_comport.c_str(), m_baudrate);
	    std::cout << MSGHDR << "Opened." << std::endl;

	  } catch (net::ysuga::ComOpenException & e) {
	    std::cout << MSGHDR << "Failed (" << e.what() << ")" << std::endl;
	    return RTC::RTC_ERROR;
	  }
	} else if(m_connectionType == "tcp") {
	  try {
	    std::cout << MSGHDR << "Connecting to " << m_ipAddress << ":" << m_portNumber << " with TCP" << std::ends;
	    m_pSerialDevice = new ssr::EtherTcp(m_ipAddress.c_str(), m_portNumber);
	    
	  } catch (...) {
	    std::cout << MSGHDR << "Failed" << std::endl;
	    return RTC::RTC_ERROR;
	  }

	}
	
	m_pRTno = new ssr::RTnoBase(this, m_pSerialDevice);

	coil::TimeValue interval(0, 1000*1000);
	std::cout << MSGHDR << "Waiting for Startup the arduino...\n";
	std::cout << MSGHDR << " - 3......" << std::endl;  coil::sleep(interval);
	std::cout << MSGHDR << " - 2...."   << std::endl;  coil::sleep(interval);
	std::cout << MSGHDR << " - 1.."     << std::endl;  coil::sleep(interval);
	std::cout << MSGHDR << " - Go!"     << std::endl;

	std::cout << MSGHDR << "Starting up onInitialize sequence." << std::endl;
	
	if (!m_pRTno->initialize()) {
	  return RTC::RTC_ERROR;
	  std::cout << MSGHDR << " - Arduino did not reply RTnoProfile request.\n"
		    << " - Please check your Arduino Board is properly connected to your machine.\n"
		    << " - Or, your RTnoProxy is correctly configured (see above message.)" << std::endl;
	}
	std::cout << MSGHDR << "RTnoProxy is successfully initialized." << std::endl;

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


