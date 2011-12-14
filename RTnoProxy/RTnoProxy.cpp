// -*- C++ -*-
/*!
 * @file  RTnoProxy.cpp
 * @brief Proxy RTC with arduino compatible device and RTnoProxy library
 * @date 11/11/2010
 * @author Yuki Suga (ysuga@ysuga.net)
 * $Id$
 */


#include "RTnoProxy.h"
#include "InPortWrapper.h"
#include <coil/Time.h>

#include "Packet.h"

enum {
  CREATED='C',
  INACTIVE='I',
  ACTIVE='A',
  RTC_ERROR='E',
  NONE='N',
};



// Module specification
// <rtc-template block="module_spec">

using namespace org::ysuga;

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
  : RTC::DataFlowComponentBase(manager)//,
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

static std::string GetStringFromPacket(const unsigned char* start_adr, int length)
{
  char* cbuf = new char[length+1];
  memcpy(cbuf, start_adr, length);
  //  cbuf[length] = 0;
  cbuf[length] = 0;
  //  std::cout << "Length = " << length << " Cbuf = " << cbuf << std::endl;
  std::string ret(cbuf);
  delete cbuf;
  return ret;
}

int RTnoProxy::addInPortWrapper(char TypeCode, const char* PortName)
{
  std::cout << "AddInPort(" << TypeCode << ", " << PortName << ")" << std::endl;
  InPortWrapperBase* inport;
  switch(TypeCode) {
  case TYPECODE_TIMED_CHAR:
	  inport = new InPortWrapper<TimedChar, char>(PortName);
	  break;
  case TYPECODE_TIMED_OCTET:
	  inport = new InPortWrapper<TimedOctet, char>(PortName);
	  break;
  case TYPECODE_TIMED_BOOLEAN:
	  inport = new InPortWrapper<TimedBoolean, bool>(PortName);
	  break;
  case TYPECODE_TIMED_LONG:
      inport = new InPortWrapper<TimedLong, long>(PortName);
    break;
  case TYPECODE_TIMED_FLOAT:
      inport = new InPortWrapper<TimedFloat, float>(PortName);
    break;
  case TYPECODE_TIMED_DOUBLE:
      inport = new InPortWrapper<TimedDouble, double>(PortName);
    break;
  case TYPECODE_TIMED_CHAR_SEQ:
	  inport = new SeqInPortWrapper<TimedCharSeq, char>(PortName);
	  break;
  case TYPECODE_TIMED_OCTET_SEQ:
	  inport = new SeqInPortWrapper<TimedOctetSeq, char>(PortName);
	  break;
  case TYPECODE_TIMED_BOOLEAN_SEQ:
	  inport = new SeqInPortWrapper<TimedBooleanSeq, bool>(PortName);  
	  break;
  case TYPECODE_TIMED_LONG_SEQ:
      inport = new SeqInPortWrapper<TimedLongSeq, long>(PortName);
    break;
  case TYPECODE_TIMED_FLOAT_SEQ:
      inport = new SeqInPortWrapper<TimedFloatSeq, float>(PortName);
    break;
  case TYPECODE_TIMED_DOUBLE_SEQ:
      inport = new SeqInPortWrapper<TimedDoubleSeq, double>(PortName);
    break;
  default:
    return -1;
  }
  std::pair<std::string, InPortWrapperBase*> portPair(std::string(PortName), inport);
  addInPort(PortName, inport->GetPort());
  inPortMap.insert(portPair);
  std::cout << "Success." << std::endl;
  return 0;
}



int RTnoProxy::addOutPortWrapper(char TypeCode, const char* PortName)
{
  std::cout << "AddOutPort(" << TypeCode << ", " << PortName << ")" << std::endl;
  OutPortWrapperBase * outport;
  switch(TypeCode) {
  case TYPECODE_TIMED_CHAR:
      outport = new OutPortWrapper<TimedChar, char>(PortName);
    break;
  case TYPECODE_TIMED_OCTET:
      outport = new OutPortWrapper<TimedOctet, char>(PortName);
    break;
  case TYPECODE_TIMED_BOOLEAN:
      outport = new OutPortWrapper<TimedBoolean, bool>(PortName);
    break;
  case TYPECODE_TIMED_LONG:
      outport = new OutPortWrapper<TimedLong, long>(PortName);
    break;
  case TYPECODE_TIMED_FLOAT:
      outport = new OutPortWrapper<TimedFloat, float>(PortName);
    break;
  case TYPECODE_TIMED_DOUBLE:
      outport = new OutPortWrapper<TimedDouble, double>(PortName);
    break;
  case TYPECODE_TIMED_CHAR_SEQ:
      outport = new SeqOutPortWrapper<TimedCharSeq, char>(PortName);
    break;
  case TYPECODE_TIMED_OCTET_SEQ:
      outport = new SeqOutPortWrapper<TimedOctetSeq, char>(PortName);
    break;
  case TYPECODE_TIMED_BOOLEAN_SEQ:
      outport = new SeqOutPortWrapper<TimedBooleanSeq, bool>(PortName);
    break;
  case TYPECODE_TIMED_LONG_SEQ:
      outport = new SeqOutPortWrapper<TimedLongSeq, long>(PortName);
    break;
  case TYPECODE_TIMED_FLOAT_SEQ:
      outport = new SeqOutPortWrapper<TimedFloatSeq, float>(PortName);
    break;
  case TYPECODE_TIMED_DOUBLE_SEQ:
      outport = new SeqOutPortWrapper<TimedDoubleSeq, double>(PortName);
    break;
  default:
    return -1;
  }
  std::pair<std::string, OutPortWrapperBase*> portPair(std::string(PortName), outport);
  addOutPort(PortName, outport->GetPort());
  outPortMap.insert(portPair);
  std::cout << "Success." << std::endl;
  return 0;
}

RTC::ReturnCode_t RTnoProxy::getRTnoProfile()
{
  std::cout << "-RTnoProxy::getRTnoProfile() called." << std::endl;
  unsigned char packet_buffer[MAX_PACKET_SIZE];

  m_pTransport->SendPacket(GET_PROFILE, 0, NULL);
  while(1) {
    int retval = m_pTransport->ReceivePacket(packet_buffer);
	if(retval == 0) continue;
	else if(retval < 0) {
      std::cout << "--RTnoProxy::getRTnoProfile() Failed." << std::endl;
	  if(retval == -CHECKSUM_ERROR) {
		std::cout << "---Check Sum Error." << std::endl;
		return RTC::RTC_ERROR;
	  } else if(retval == -TIMEOUT) {
		std::cout << "---Timeout Error." << std::endl;
		return RTC::RTC_ERROR;
	  } else {
		std::cout << "---Unknown Error ("<<retval<<")" << std::endl;
		return RTC::RTC_ERROR;
	  }
	}


    std::cout << "--Packet Received." << std::endl;

    std::string strbuf;
    switch(packet_buffer[PACKET_INTERFACE]) {
    case GET_PROFILE: // Return Code.
		if(packet_buffer[DATA_START_ADDR] == RTNO_OK) {
			std::cout << "--RTnoProxy::getRTnoProfile() Succeeded." << std::endl;
			return RTC::RTC_OK;
		} else {
			std::cout << "--RTnoProxy::getRTnoProfile() Failed." << std::endl;
			return RTC::RTC_ERROR;
		}
		break;
    case ADD_INPORT: 
      strbuf = GetStringFromPacket(packet_buffer+DATA_START_ADDR+1, packet_buffer[DATA_LENGTH]-1);
      addInPortWrapper(packet_buffer[DATA_START_ADDR], strbuf.c_str());
      break;
    case ADD_OUTPORT:
      strbuf = GetStringFromPacket(packet_buffer+DATA_START_ADDR+1, packet_buffer[DATA_LENGTH]-1);
      addOutPortWrapper(packet_buffer[DATA_START_ADDR], strbuf.c_str());
      break;
    default:
      std::cout << "Unknown Command (" << packet_buffer[PACKET_INTERFACE] << ")" << std::endl;
      return RTC::RTC_ERROR;
    }
  }
}

RTC::ReturnCode_t RTnoProxy::getStatus()
{
	unsigned char packet_buffer[MAX_PACKET_SIZE];
	m_pTransport->SendPacket(GET_STATUS, 0, NULL);
	m_pTransport->ReceivePacket(packet_buffer);
  if(packet_buffer[PACKET_INTERFACE] != GET_STATUS) {
	  return RTC::RTC_ERROR;
  }
  switch(packet_buffer[DATA_START_ADDR]) {
	  case INACTIVE:
		  std::cout << "RTno is inactive. OK." << std::endl;
		  return RTC::RTC_OK;
	  case ACTIVE:
		  std::cout << "RTno is active." << std::endl;
		  std::cout << "deactivating the RTno....." << std::ends;
		  m_pTransport->SendPacket(DEACTIVATE, 0, NULL);
		  m_pTransport->ReceivePacket(packet_buffer);
		  if(packet_buffer[DATA_START_ADDR] == RTC_OK) {
			  std::cout << "OK." << std::endl;
			  return RTC::RTC_OK;
		  } else {
			  return RTC::RTC_ERROR;
		  }
	  default:
		  std::cout << "RTno is unknown state." << std::endl;
		  return RTC::RTC_ERROR;
  }

  return RTC::RTC_OK;
}

RTC::ReturnCode_t RTnoProxy::getContext()
{
	unsigned char packet_buffer[MAX_PACKET_SIZE];
  m_pTransport->SendPacket(GET_CONTEXT, 0, NULL);
  m_pTransport->ReceivePacket(packet_buffer);
  if(packet_buffer[PACKET_INTERFACE] != GET_CONTEXT) {
	  return RTC::RTC_ERROR;
  }
  switch(packet_buffer[DATA_START_ADDR]) {
	  case ProxySynchronousExecutionContext:
		  m_ProxySynchronousExecution = TRUE;
		  break;

	  default:
		  m_ProxySynchronousExecution = FALSE;
  }

  return RTC::RTC_OK;
}


RTC::ReturnCode_t RTnoProxy::onInitialize()
{
  bindParameter("comport", m_comport, "/dev/tty0");
  bindParameter("baudrate", m_baudrate, "19200");
  updateParameters("default");
  std::cout << "conf.default.comport" << ":" << m_comport << std::endl;
  std::cout << "conf.default.baudrate" << ":" << m_baudrate << std::endl;
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  
  // Set OutPort buffer
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  std::cout << "Opening SerialPort(" << m_comport << ")....." << std::ends;
  try {
    //  m_pSerialPort = new SerialPort(m_comport.c_str(), m_baudrate);
	  m_pTransport = new UARTTransport(m_comport.c_str(), m_baudrate);
  } catch (ComOpenException& e) {
    std::cout << "Fail" << std::endl;
    return RTC::RTC_ERROR;
  }
  std::cout << "OK." << std::endl;

  coil::TimeValue interval(0, 1000*1000);
  std::cout << "Waiting for Startup the arduino...\n3..." << std::endl;
  coil::sleep(interval);

  std::cout << "2.." << std::endl;
  coil::sleep(interval);
  std::cout << "1.." << std::endl;
  coil::sleep(interval);
  std::cout << "Go!" << std::endl;

  std::cout << "Starting up onInitialize sequence." << std::endl;
  getRTnoProfile(); 

  getStatus();

  getContext();

  throw new std::exception();

  return RTC::RTC_OK;
}

RTC::ReturnCode_t RTnoProxy::onFinalize()
{
	delete m_pTransport;
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
  std::cout << "Starting onActivated sequence."<< std::ends;
  m_pTransport->SendPacket(ACTIVATE, 0, NULL);
  unsigned char packet_buffer[MAX_PACKET_SIZE];
  while(1) {
    
    int ret = m_pTransport->ReceivePacket(packet_buffer);
	if(ret == 0) continue;
    if(packet_buffer[PACKET_INTERFACE] == ACTIVATE) {
		if(packet_buffer[DATA_START_ADDR] == RTNO_OK) {
	std::cout << "Success" << std::endl;
	return RTC::RTC_OK;
      }else {
	return RTC::RTC_ERROR;
      }
    }else {
      std::cout << "Unknown Interface: " << packet_buffer[PACKET_INTERFACE] << std::endl;
     // return RTC::RTC_ERROR;
    }
  }
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RTnoProxy::onDeactivated(RTC::UniqueId ec_id)
{
  std::cout << "Starting onDeactivated sequence."<< std::endl;
  m_pTransport->SendPacket(DEACTIVATE, 0, NULL);
  unsigned char packet_buffer[MAX_PACKET_SIZE];
  while(1) {
    m_pTransport->ReceivePacket(packet_buffer);
    if(packet_buffer[PACKET_INTERFACE] == DEACTIVATE) {
      if(packet_buffer[DATA_START_ADDR] == RTNO_OK) {
	std::cout << "Success" << std::endl;
	return RTC::RTC_OK;
      }else {
	return RTC::RTC_ERROR;
      }
    }else {
      std::cout << "Unknown Interface: " << packet_buffer[PACKET_INTERFACE] << std::endl;
      return RTC::RTC_ERROR;
    }
  }
  return RTC::RTC_ERROR;
}


RTC::ReturnCode_t RTnoProxy::onExecute(RTC::UniqueId ec_id)
{
  //  std::cout << "Starting onExecute sequence."<< std::endl;
  unsigned char packet_buffer[MAX_PACKET_SIZE];
  char nameBuffer[32];
  unsigned char ret;
  std::string strbuf;
  InPortWrapperBase* inport;
  OutPortWrapperBase* outport;
  int len, namelen, datalen;


  std::map<std::string, InPortWrapperBase*>::iterator it = inPortMap.begin();
  while(it != inPortMap.end()) {
	  std::string name = (*it).first;
	  InPortWrapperBase* inPort = (*it).second;
	  if(inPort->isNew()) {
		  int len = inPort->Read();
		  int namelen = strlen(name.c_str());
		  packet_buffer[0] = namelen;
		  packet_buffer[1] = inPort->getTypeSizeInArduino() * len;
		  memcpy(packet_buffer+2, name.c_str(), namelen);
		  inPort->Get(packet_buffer+2+namelen, len);
		  m_pTransport->SendPacket(SEND_DATA, len*inPort->getTypeSizeInArduino() + namelen + 2, packet_buffer);
	  }
	  ++it;
  }


  m_pTransport->SendPacket(EXECUTE, 0, NULL);

  while(1) {  
	  int ret = m_pTransport->ReceivePacket(packet_buffer);
	  if(ret == 0) continue;
	  if(ret > 0) {
		  switch(packet_buffer[PACKET_INTERFACE]) {
			  case RECEIVE_DATA: 
				{
				  namelen = packet_buffer[DATA_START_ADDR];
				  datalen = packet_buffer[DATA_START_ADDR+1];
				  memcpy(nameBuffer, &(packet_buffer[DATA_START_ADDR+2]), namelen);
				  nameBuffer[namelen] = 0;
				  OutPortWrapperBase* outPort = outPortMap[nameBuffer];
				  if(outPort == NULL) {
					  std::cout << "Unknown Port Name [" << nameBuffer << "]" << std::endl;
				  } else {
					  outPort->Write(packet_buffer + DATA_START_ADDR + 2 + namelen, datalen / outPort->getTypeSizeInArduino());
				  }
				}
				  break;
			  case EXECUTE:
				  if(packet_buffer[DATA_START_ADDR] == RTNO_OK) {
				return RTC::RTC_OK;
				  }else {
				return RTC::RTC_ERROR;
				  }

				  break;
			  default:
				  std::cout << "Unknown Packet [" << packet_buffer[PACKET_INTERFACE] << "]" << std::endl;
				  break;
		  }

	  }
	  if(ret < 0) {

	  }
  }

/**
 // int ret;
  while((ret = ReceivePacket(m_pSerialPort, packet_buffer)) == 0) {
	  ; // do nothing;
  }
  if(ret < 0) {
	  return RTC::RTC_ERROR;
  }
  if(packet_buffer[PACKET_INTERFACE] == EXECUTE) {
      if(packet_buffer[DATA_START_ADDR] == RTNO_OK) {
	return RTC::RTC_OK;
      }else {
	return RTC::RTC_ERROR;
      }
  } else {
	  return RTC::RTC_ERROR;
  }

  /*	  
  while(1) {
    int retval = ReceivePacket(m_pSerialPort, packet_buffer);
	if(retval < 0) {
		// Timeout
		return RTC::RTC_OK;
	}
	
      // go to return RTC::RTC_OK;
    case INPORT_ISNEW:

      strbuf = GetStringFromPacket(packet_buffer + DATA_START_ADDR, packet_buffer[DATA_LENGTH]);
      inport = inPortMap[strbuf];
      if(inport == NULL) {
	ret = RTNO_ERROR;
      } else {
	ret = inport->isNew();
      }
      //      std::cout << "InPort(" << strbuf << ").isNew() = " << ret <<  std::endl;
      SendPacket(m_pSerialPort, INPORT_ISNEW, 1, &ret);
      break;
    case INPORT_READ:
      strbuf = GetStringFromPacket(packet_buffer + DATA_START_ADDR, packet_buffer[DATA_LENGTH]);
      inport = inPortMap[strbuf];
      len = inport->Read();
      //std::cout << "InPOrt(" << strbuf << ").read() size=" << len << std::endl;
      inport->Get(packet_buffer, len);
      SendPacket(m_pSerialPort, INPORT_READ, inport->getTypeSizeInArduino()*len, packet_buffer);
      break;
    case OUTPORT_WRITE:

      namelen = packet_buffer[DATA_START_ADDR + 0];
      datalen = packet_buffer[DATA_START_ADDR + 1];
      strbuf = GetStringFromPacket(packet_buffer + DATA_START_ADDR+2, namelen);      
      //      std::cout << "OutPort(" << strbuf << ", " << datalen <<  ").write()" << std::endl;
      outport = outPortMap[strbuf];

      if(outport == NULL) {
	std::cout << "--null ptr" << std::endl;
	ret = RTNO_ERROR;
      } else {
	outport->Write(packet_buffer + DATA_START_ADDR + 2 + namelen, datalen / outport->getTypeSizeInArduino());	
	ret = RTNO_OK;
      }
      SendPacket(m_pSerialPort, OUTPORT_WRITE, 1, &ret);
      break;
    default:
      std::cout << "Unknown Command : " << packet_buffer[PACKET_INTERFACE] << std::endl;
      return RTC::RTC_ERROR;
    }
  }
   */
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


