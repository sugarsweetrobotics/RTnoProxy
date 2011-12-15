#pragma once

#include <rtm/DataFlowComponentBase.h>
#include <map>

#include "InPortWrapper.h"
#include "OutPortWrapper.h"

#include "RTnoProfile.h"

typedef std::map<std::string, InPortWrapperBase*> InPortMap;
typedef std::map<std::string, InPortWrapperBase*>::iterator InPortMapIterator;
typedef std::map<std::string, OutPortWrapperBase*> OutPortMap;
typedef std::map<std::string, OutPortWrapperBase*>::iterator OutPortMapIterator;

namespace org {
	namespace ysuga {
class RTnoRTObjectWrapper
{
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

public:
	int AddInPortWrapper(char TypeCode, const char* PortName);
	int AddOutPortWrapper(char TypeCode, const char* PortName);

	int AddInPort(org::ysuga::PortProfile* profile) {
		return AddInPortWrapper(profile->GetTypeCode(), profile->GetPortName());
	}

	int AddOutPort(org::ysuga::PortProfile* profile) {
		return AddOutPortWrapper(profile->GetTypeCode(), profile->GetPortName());
	}

	OutPortWrapperBase* GetOutPort(const char* portName) {
		return m_OutPortMap[portName];
	}

	InPortWrapperBase* GetInPort(const char* portName) {
		return m_InPortMap[portName];
	}

};

	};
};
