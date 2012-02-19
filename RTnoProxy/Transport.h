#pragma once

#include "PacketBuffer.h"
#include "SerialDevice.h"

namespace net {
  namespace ysuga { 
    class Transport
    {
    private:
      PacketBuffer *m_pPacketBuffer;
    protected:
      SerialDevice *m_pSerialDevice;
      
    public:
      Transport(SerialDevice *pSerialDevice);
      ~Transport(void);
      
      
    public:
      int SendPacket(char* address, PacketBuffer* packet);

      PacketBuffer* ReceivePacket();
      int ReceivePacket(unsigned char* packet);
      int IsReceived();
    private:
      int _Wait(unsigned char buffer_size);
    };
  };
};
