#pragma once

#include "Transport.h"
#include "RTnoProfile.h"



namespace net {
  namespace ysuga {
    class RTnoRTObjectWrapper;
    
    class RTnoProtocol
    {
    private:
      int m_SendBusy;
      Transport *m_pTransport;
      RTnoRTObjectWrapper *m_pRTObjectWrapper;
    public:
      RTnoProtocol(RTnoRTObjectWrapper *pRTObject, Transport* pTransport);
      virtual ~RTnoProtocol(void);
      
    public:
      int GetRTnoProfile(RTnoProfile* profile);
      unsigned char GetRTnoStatus();
      unsigned char GetRTnoExecutionContextType();
      
      int ActivateRTno();
      int DeactivateRTno();
      int ResetRTno();

      int ConnectInPort(const char* sourceAddr, const uint8_t sourcePort, const char* targetAddress, const uint8_t targetPort);
      int ConnectOutPort(const char* sourceAddr, const uint8_t sourcePort, const char* targetAddress, const uint8_t targetPort);
      
      int SendData(const uint8_t index, const unsigned char* data, int length);
      int ReceiveData(PacketBuffer * packet);
      int SendExecuteTrigger();
      
      int ReceiveReturnCode(unsigned char intf);
      
      int HandleReceivedPacket(void);


    private:
      std::string GetStringFromPacket(const int8_t* start_adr, int length);
      void SendRequest(int8_t interface);
    };
    
  };
};
