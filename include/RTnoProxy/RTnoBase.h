#pragma once

#include "Thread.h"
#include "RTnoProfile.h"

namespace ssr {
    class SerialDevice;
    class RTnoRTObjectWrapper;
    class Transport;
    class RTnoProtocol;
};


namespace RTC {
  class DataFlowComponentBase;
};

namespace ssr {


  class RTnoBase : public net::ysuga::Thread {
  private:
    ssr::SerialDevice *m_pSerialDevice;
    ssr::RTnoProfile m_Profile;
    ssr::RTnoRTObjectWrapper *m_pRTObjectWrapper;
    ssr::Transport* m_pTransport;
    ssr::RTnoProtocol* m_pProtocol;
    RTC::DataFlowComponentBase* m_pRTC;

  public:
    RTnoBase(RTC::DataFlowComponentBase* pRTC, ssr::SerialDevice* pSerial);

    virtual ~RTnoBase();

    
    bool initialize();

    bool activate();

    bool deactivate();

    bool execute();

    bool reset();
  };
}
