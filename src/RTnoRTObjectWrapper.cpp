#include "stdafx.h"

/*
#include <rtm/Manager.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>

#include <rtm/idl/BasicDataType.hh>
*/

#if defined(_MSC_VER) && (_MSC_VER <= 1500)
	typedef __int8  int8_t;
	typedef unsigned __int8 uint8_t;
	typedef __int16 int16_t;
	typedef unsigned __int16 uint16_t;
	typedef __int32 int32_t;
	typedef unsigned __int32 uint32_t;
    typedef __int64 int64_t;
    typedef unsigned __int64 uint64_t;
#else
    #include <stdint.h>
#endif

#include "RTnoRTObjectWrapper.h"

#include "Packet.h"

using namespace RTC;
using namespace net::ysuga;

RTnoRTObjectWrapper::RTnoRTObjectWrapper(RTC::DataFlowComponentBase* pRTC)
{
	m_pRTC = pRTC;
}

RTnoRTObjectWrapper::~RTnoRTObjectWrapper(void)
{
}



int RTnoRTObjectWrapper::AddInPortWrapper(char TypeCode, const char* PortName)
{
#ifdef DEBUG
  std::cout << "-RTnoRTObjectWrapper::AddInPortWrapper(" << TypeCode << ", " << PortName << ")" << std::endl;
#endif
	InPortWrapperBase* inport;
	switch(TypeCode) {
	case TYPECODE_TIMED_CHAR:
	  inport = new InPortWrapper<TimedChar, char>(PortName);
	  break;
	case TYPECODE_TIMED_OCTET:
	  inport = new InPortWrapper<TimedOctet, int8_t>(PortName);
	  break;
	case TYPECODE_TIMED_BOOLEAN:
	  inport = new InPortWrapper<TimedBoolean, bool>(PortName);
	  break;
	case TYPECODE_TIMED_LONG:
	  inport = new InPortWrapper<TimedLong, int32_t>(PortName);
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
	  inport = new SeqInPortWrapper<TimedOctetSeq, int8_t>(PortName);
	  break;
	case TYPECODE_TIMED_BOOLEAN_SEQ:
	  inport = new SeqInPortWrapper<TimedBooleanSeq, bool>(PortName);  
	  break;
	case TYPECODE_TIMED_LONG_SEQ:
	  inport = new SeqInPortWrapper<TimedLongSeq, int32_t>(PortName);
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
	m_pRTC->addInPort(PortName, inport->GetPort());
	m_InPortMap.insert(portPair);
#ifdef DEBUG
	std::cout << "---Success." << std::endl;
#endif
	return 0;
}




int RTnoRTObjectWrapper::AddOutPortWrapper(char TypeCode, const char* PortName)
{
#ifdef DEBUG
  std::cout << "-RTnoRTObjectWrapper::AddOutPortWrapper(" << TypeCode << ", " << PortName << ")" << std::endl;
#endif
  OutPortWrapperBase * outport;
  switch(TypeCode) {
  case TYPECODE_TIMED_CHAR:
      outport = new OutPortWrapper<TimedChar, char>(PortName);
    break;
  case TYPECODE_TIMED_OCTET:
      outport = new OutPortWrapper<TimedOctet, int8_t>(PortName);
    break;
  case TYPECODE_TIMED_BOOLEAN:
      outport = new OutPortWrapper<TimedBoolean, bool>(PortName);
    break;
  case TYPECODE_TIMED_LONG:
    outport = new OutPortWrapper<TimedLong, int32_t>(PortName);
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
      outport = new SeqOutPortWrapper<TimedOctetSeq, int8_t>(PortName);
    break;
  case TYPECODE_TIMED_BOOLEAN_SEQ:
      outport = new SeqOutPortWrapper<TimedBooleanSeq, bool>(PortName);
    break;
  case TYPECODE_TIMED_LONG_SEQ:
      outport = new SeqOutPortWrapper<TimedLongSeq, int32_t>(PortName);
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
  m_pRTC->addOutPort(PortName, outport->GetPort());
  m_OutPortMap.insert(portPair);
#ifdef DEBUG
  std::cout << "---Success." << std::endl;
#endif
  return 0;
}
