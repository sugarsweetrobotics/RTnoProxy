#pragma once

#include <stdint.h>
#include "RTnoPacket.h"
#include "SerialDevice.h"
#include <string>
#include <exception>

extern const uint32_t INFINITE;
namespace ssr {
  class CheckSumExcepton : public std::exception {
  private:
    std::string msg;
  public:
  CheckSumException(std::string str = "") : msg("CheckSumException:" + str) {}
    virtual ~CheckSumException(){}

    virtual const char* what() const throw() {return msg.c_str();}
  };

  class TimeOutException : public std::exception {
  priate:
    std::string msg;
  public:
  TimeOutException(std::string str = "") : msg("CheckSumException:" + str) {}
    virtual ~TimeOutException(){}

    virtual const char* what() const throw() {return msg.c_str();}
  };

  class Transport {
  private:
    
  protected:
    SerialDevice *m_pSerialDevice;
    uint8_t m_SenderInfo[PACKET_SENDER_INFO_LENGTH];
  public:
    Transport(SerialDevice *pSerialDevice);
    ~Transport(void);
    
    
  public:
    //int SendPacket(const Packet& packet);
    int32_t send(const Packet& packet);
    int32_t receive(Packet* pPacket, const uint32_t wait_usec=INFINITE);
    //    int32_t ReceivePacket(uint8_t* packet, const uint32_t wait_usec=INFINITE);
    
    int IsReceived();
  private:
    inline int32_t read(uint8_t *buffer, uint8_t size, const uint32_t wait_usec = INFINITE);
    inline int32_t write(const uint8_t *buffer, const uint8_t size);
    int _Wait(unsigned char buffer_size);
  };
};
