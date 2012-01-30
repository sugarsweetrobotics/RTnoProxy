// -*- C++ -*-
/*!
 * @file  DataPortTest.cpp
 * @brief DataPortTest
 * @date $Date$
 *
 * $Id$
 */

#include "DataPortTest.h"


// Module specification
// <rtc-template block="module_spec">
static const char* dataporttest_spec[] =
  {
    "implementation_id", "DataPortTest",
    "type_name",         "DataPortTest",
    "description",       "DataPortTest",
    "version",           "1.0.0",
    "vendor",            "ysuga.net",
    "category",          "Test",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
DataPortTest::DataPortTest(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_longInIn("longIn", m_longIn),
    m_floatInIn("floatIn", m_floatIn),
    m_doubleInIn("doubleIn", m_doubleIn),
    m_longSeqInIn("longSeqIn", m_longSeqIn),
    m_floatSeqInIn("floatSeqIn", m_floatSeqIn),
    m_doubleSeqInIn("doubleSeqIn", m_doubleSeqIn),
    m_longOutOut("longOut", m_longOut),
    m_floatOutOut("floatOut", m_floatOut),
    m_doubleOutOut("doubleOut", m_doubleOut),
    m_longSeqOutOut("longSeqOut", m_longSeqOut),
    m_floatSeqOutOut("floatSeqOut", m_floatSeqOut),
    m_doubleSeqOutOut("doubleSeqOut", m_doubleSeqOut)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
DataPortTest::~DataPortTest()
{
}



RTC::ReturnCode_t DataPortTest::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("longIn", m_longInIn);
  addInPort("floatIn", m_floatInIn);
  addInPort("doubleIn", m_doubleInIn);
  addInPort("longSeqIn", m_longSeqInIn);
  addInPort("floatSeqIn", m_floatSeqInIn);
  addInPort("doubleSeqIn", m_doubleSeqInIn);
  
  // Set OutPort buffer
  addOutPort("longOut", m_longOutOut);
  addOutPort("floatOut", m_floatOutOut);
  addOutPort("doubleOut", m_doubleOutOut);
  addOutPort("longSeqOut", m_longSeqOutOut);
  addOutPort("floatSeqOut", m_floatSeqOutOut);
  addOutPort("doubleSeqOut", m_doubleSeqOutOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t DataPortTest::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DataPortTest::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DataPortTest::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DataPortTest::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DataPortTest::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t DataPortTest::onExecute(RTC::UniqueId ec_id)
{
	std::cout << "Data Port Test" << std::endl;
	std::cout << "l:longOut\nf:floatOut\nd:doubleOut\n";
	std::cout << "L:longSeqOut\nF:floatSeqOut\nD:doubleSeqOut\n";
	std::cout <<  "v:Just view received data.\n";
	std::cout << "Input Character and press Enter key >> " << std::endl;


	char c;
	char buffer[256];
	std::cin.clear();
	std::cin >> c;
	std::cin.clear();
	std::cin.ignore( 1024, '\n' ); 
	switch(c) {
		case 'l':
			std::cout << "Input Number (long): ";
			std::cin  >> buffer;
			m_longOut.data = atoi(buffer);
			m_longOutOut.write();
			break;
		case 'f':
			std::cout << "Input Number (float): ";
			std::cin  >> buffer;
			m_floatOut.data = atof(buffer);
			m_floatOutOut.write();
			break;
		case 'd':
			std::cout << "Input Number (double): ";
			std::cin  >> buffer;
			m_doubleOut.data = atof(buffer);
			m_doubleOutOut.write();
			break;

		case 'L':
			std::cout << "Input Length (long): ";
			std::cin  >> buffer;
			m_longSeqOut.data.length(atoi(buffer));
			for(unsigned int i = 0;i < m_longSeqOut.data.length();i++) {
				std::cin >> buffer;
				m_longSeqOut.data[i] = atoi(buffer);
			}
			m_longSeqOutOut.write();
			break;
		case 'F':
			std::cout << "Input Length (float): ";
			std::cin  >> buffer;
			m_floatSeqOut.data.length(atoi(buffer));
			for(unsigned int i = 0;i < m_floatSeqOut.data.length();i++) {
				std::cin >> buffer;
				m_floatSeqOut.data[i] = atof(buffer);
			}
			m_floatSeqOutOut.write();
			break;
		case 'D':
			std::cout << "Input Length (double): ";
			std::cin  >> buffer;
			m_doubleSeqOut.data.length(atoi(buffer));
			for(unsigned int i = 0;i < m_doubleSeqOut.data.length();i++) {
				std::cin >> buffer;
				m_doubleSeqOut.data[i] = atof(buffer);
			}
			m_doubleSeqOutOut.write();
			break;
		case 'v':
			break;
		default:
			break;
	}

	if(m_floatInIn.isNew()) {
		m_floatInIn.read();
	}
	if(m_longInIn.isNew()) {
		m_longInIn.read();
	}
	if(m_doubleInIn.isNew()) {
		m_doubleInIn.read();
	}
	if(m_floatSeqInIn.isNew()) {
		m_floatSeqInIn.read();
	}
	if(m_longSeqInIn.isNew()) {
		m_longSeqInIn.read();
	}
	if(m_doubleSeqInIn.isNew()) {
		m_doubleSeqInIn.read();
	}
	std::cout << "Long  :" << m_longIn.data << std::endl;
	std::cout << "Float :" << m_floatIn.data << std::endl;
	std::cout << "Double:" << m_doubleIn.data << std::endl;

	std::cout << "LongSeq[" << m_longSeqIn.data.length() << "]:" << std::endl;
	for(unsigned int i = 0;i < m_longSeqIn.data.length();i++) {
		std::cout << m_longSeqIn.data[i] << ", ";
	}
	std::cout << std::endl;
	std::cout << "FloatSeq:" << std::endl;
	for(unsigned int i = 0;i < m_floatSeqIn.data.length();i++) {
		std::cout << m_floatSeqIn.data[i] << ", ";
	}
	std::cout << std::endl;
	std::cout << "Double:" << std::endl;
	for(unsigned int i = 0;i < m_doubleSeqIn.data.length();i++) {
		std::cout << m_doubleSeqIn.data[i] << ", ";
	}
	std::cout << std::endl;


  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t DataPortTest::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DataPortTest::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DataPortTest::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DataPortTest::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DataPortTest::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void DataPortTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(dataporttest_spec);
    manager->registerFactory(profile,
                             RTC::Create<DataPortTest>,
                             RTC::Delete<DataPortTest>);
  }
  
};


