#pragma once

#include <map>

namespace RTC {
  class DataFlowComponentBase;
}

#include "RTnoProfile.h"

#include "InPortWrapper.h"
#include "OutPortWrapper.h"

typedef std::map<std::string, InPortWrapperBase*> InPortMap;
typedef std::map<std::string, InPortWrapperBase*>::iterator InPortMapIterator;
typedef std::map<std::string, OutPortWrapperBase*> OutPortMap;
typedef std::map<std::string, OutPortWrapperBase*>::iterator OutPortMapIterator;

namespace ssr {

  class UnknownTypeCodeException : public std::exception {
  private:
    std::string msg;
  public:
  UnknownTypeCodeException(std::string str = "") : msg("UnknownTypeCodeException:" + str) {}
    virtual ~UnknownTypeCodeException() throw() {}

    virtual const char* what() const throw() {return msg.c_str();}
  };

  class RTnoRTObjectWrapper {
  private:
    RTC::DataFlowComponentBase* m_pRTC;
    
  private:
    InPortMap m_InPortMap;
    OutPortMap m_OutPortMap;
    
  public:
    InPortMap* GetInPortMap() {return &m_InPortMap;}
    OutPortMap* GetOutPortMap() {return &m_OutPortMap;}
    
  public:
    RTnoRTObjectWrapper(RTC::DataFlowComponentBase* pRTC);
    virtual ~RTnoRTObjectWrapper(void);
    
  private:
    int32_t AddInPortWrapper(const uint8_t TypeCode, const std::string& PortName);
    int32_t AddOutPortWrapper(const uint8_t TypeCode, const std::string& PortName);
  public:    
    int32_t addInPort(PortProfile& profile) {
      return AddInPortWrapper(profile.getTypeCode(), profile.getPortName());
    }
    
    int32_t addOutPort(PortProfile& profile) {
      return AddOutPortWrapper(profile.getTypeCode(), profile.getPortName());
    }
    
    OutPortWrapperBase* GetOutPort(const char* portName) {
      return m_OutPortMap[portName];
    }
    
    InPortWrapperBase* GetInPort(const char* portName) {
      return m_InPortMap[portName];
    }

  };
};
