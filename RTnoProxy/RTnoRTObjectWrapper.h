#pragma once

#include <map>

#include "RTnoProfile.h"

typedef std::map<std::string, InPortWrapperBase*> InPortMap;
typedef std::map<std::string, InPortWrapperBase*>::iterator InPortMapIterator;
typedef std::map<std::string, OutPortWrapperBase*> OutPortMap;
typedef std::map<std::string, OutPortWrapperBase*>::iterator OutPortMapIterator;

namespace net {
	namespace ysuga {
class RTnoRTObjectWrapper
{
private:
	RTC::DataFlowComponentBase* m_pRTC;
	
private:
	InPortMap m_InPortMap;
	OutPortMap m_OutPortMap;
	std::vector<std::string> m_InPortNameList;
	std::vector<std::string> m_OutPortNameList;

public:
	InPortMap* GetInPortMap() {return &m_InPortMap;}
	OutPortMap* GetOutPortMap() {return &m_OutPortMap;}

public:
	RTnoRTObjectWrapper(RTC::DataFlowComponentBase* pRTC);
	virtual ~RTnoRTObjectWrapper(void);

public:
	int AddInPortWrapper(char TypeCode, const char* PortName);
	int AddOutPortWrapper(char TypeCode, const char* PortName);

	int GetNumInPort() {
	  return m_InPortMap.size();
	}

	int GetNumOutPort() {
	  return m_OutPortMap.size();
	}

	int AddInPort(PortProfile* profile) {
		return AddInPortWrapper(profile->GetTypeCode(), profile->GetPortName());
	}

	int AddOutPort(PortProfile* profile) {
		return AddOutPortWrapper(profile->GetTypeCode(), profile->GetPortName());
	}

	OutPortWrapperBase* GetOutPort(const char* portName) {
		return m_OutPortMap[portName];
	}

	InPortWrapperBase* GetInPort(const char* portName) {
		return m_InPortMap[portName];
	}
	
	OutPortWrapperBase* GetOutPort(int index) {
	  return m_OutPortMap[m_OutPortNameList[index].c_str()];
	}

	InPortWrapperBase* GetInPort(int index) {
	  return m_InPortMap[m_InPortNameList[index].c_str()];
	}
};

	};
};
