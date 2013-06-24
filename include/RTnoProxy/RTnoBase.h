#pragma once

#include "RTnoProfile.h"

namespace net {
  namespace ysuga { 
    class SerialDevice;
    class RTnoRTObjectWrapper;
    class Transport;
    class RTnoProtocol;
  };
};


namespace RTC {
  class DataFlowComponentBase;
};

namespace ssr {


  class RTnoBase {
  private:
    net::ysuga::SerialDevice *m_pSerialDevice;
    net::ysuga::RTnoProfile m_Profile;
    net::ysuga::RTnoRTObjectWrapper *m_pRTObjectWrapper;
    net::ysuga::Transport* m_pTransport;
    net::ysuga::RTnoProtocol* m_pProtocol;
    RTC::DataFlowComponentBase* m_pRTC;
    bool m_ProxySynchronousExecution;
  public:
    RTnoBase(RTC::DataFlowComponentBase* pRTC, net::ysuga::SerialDevice* pSerial);

    virtual ~RTnoBase();
    
    bool initialize();

    bool activate();

    bool deactivate();

    bool execute();

    bool reset();
  };
}
