#pragma once

#include <string.h>
#include <list>


namespace ssr {
  class PortProfile {
  private:
    uint8_t m_TypeCode;
    std::string m_PortName;
  public:
    PortProfile(uint8_t typeCode, const char* portName) {
      m_TypeCode = typeCode;
      m_PortName = portName;
    }

    PortProfile(const PortProfile &p) {
      m_TypeCode = p.m_TypeCode;
      m_PortName = p.m_PortName;
    }
    
    virtual ~PortProfile() {}
    
    const uint8_t& getTypeCode() {
      return m_TypeCode;
    }
    
    const std::string& getPortName() const {
      return m_PortName;
    }

    const bool operator==(const PortProfile& p) {
      return (getPortName() == p.getPortName());
    }
  };
  
  typedef std::list<PortProfile> PortList;
  typedef std::list<PortProfile>::iterator PortListIterator;
  
  
  class RTnoProfile {
  private:
    PortList m_InPorts;
    PortList m_OutPorts;
  public:
    
    RTnoProfile(void) {}

    RTnoProfile(const RTnoProfile& p) {
      m_InPorts = p.m_InPorts;
      m_OutPorts = p.m_OutPorts;
    }
    
    virtual ~RTnoProfile(void) {}
    
  public:
    
    void appendInPort(const PortProfile& p) {
      m_InPorts.push_back(p);
    }

    void appendOutPort(const PortProfile& p) {
      m_OutPorts.push_back(p);
    }

    
    const PortList& inPorts() {
      return m_InPorts;
    }
    
    const PortList& outPorts() {
      return m_OutPorts;
    }
    
    
    void removeInPort(const char* portName) {
      for(std::list<PortProfile>::iterator it = m_InPorts.begin(); it != m_InPorts.end(); ++it) {
	if((*it).getPortName() == portName) { 
	  m_InPorts.remove((*it));
	  return;
	}
      }
    }
    
    void removeOutPort(const char* portName) {
      for(std::list<PortProfile>::iterator it = m_OutPorts.begin(); it != m_OutPorts.end(); ++it) {
	if((*it).getPortName() == portName) {
	  m_OutPorts.remove((*it));
	  return;
	}
      }
    }
    
  };
};
