#include "stdafx.h"

#include <stdint.h>
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
  std::cout << "AddInPort(" << TypeCode << ", " << PortName << ")" << std::endl;
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
  m_InPortNameList.push_back(std::string(PortName));
  std::cout << "Success." << std::endl;
  return 0;
}




int RTnoRTObjectWrapper::AddOutPortWrapper(char TypeCode, const char* PortName)
{
  std::cout << "AddOutPort(" << TypeCode << ", " << PortName << ")" << std::endl;
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
  m_OutPortNameList.push_back(std::string(PortName));
  std::cout << "Success." << std::endl;
  return 0;
}
