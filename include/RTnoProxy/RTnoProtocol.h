#pragma once

#include <stdint.h>
#include <string>
#include <exception>
#include "Transport.h"
#include "RTnoProfile.h"



namespace ssr {
  class GetProfileException : public std::exception {
  private:
    std::string msg;
  public:
  GetProfileException(std::string str = "") : msg("GetProfileException:" + str) {}
    virtual ~GetProfileException() throw() {}

    virtual const char* what() const throw() {return msg.c_str();}
  };

  class ExecuteFailedException : public std::exception {
  private:
    std::string msg;
  public:
  ExecuteFailedException(std::string str = "") : msg("ExecuteFailedException:" + str) {}
    virtual ~ExecuteFailedException() throw() {}

    virtual const char* what() const throw() {return msg.c_str();}
  };

  class UnknownOutPortRequestedException : public std::exception {
  private:
    std::string msg;
  public:
  UnknownOutPortRequestedException(std::string str = "") : msg("UnknownOutPortRequestedException:" + str) {}
    virtual ~UnknownOutPortRequestedException() throw() {}

    virtual const char* what() const throw() {return msg.c_str();}
  };


  class RTnoRTObjectWrapper;

  class RTnoProtocol {
  private:
    Transport *m_pTransport;
    RTnoRTObjectWrapper *m_pRTObjectWrapper;
    RTnoProfile m_Profile;

    bool m_ProxySynchronousExecution;
  public:
    
  public:
    RTnoProtocol(RTnoRTObjectWrapper *pRTObject, Transport* pTransport);
    virtual ~RTnoProtocol(void);
    
  public:
    RTnoPacket waitCommand(const uint8_t command, const uint32_t wait_usec);
    const RTnoProfile& getRTnoProfile(const uint32_t wait_usec);

  private:
    void onGetProfile(const RTnoPacket& packet);
    void onAddInPort(const RTnoPacket& packet);
    void onAddOutPort(const RTnoPacket& packet);
    
    PortProfile parsePortProfile(const RTnoPacket& packet);

  public:
    uint8_t getRTnoStatus();
  public:
    unsigned char getRTnoExecutionContextType();
    uint8_t initialize();
    uint8_t activate();
    uint8_t deactivate();
    uint8_t reset();
    
    uint8_t sendData(const std::string& portName, const uint8_t* data, const uint8_t length);
    int32_t sendExecuteTrigger();

    void handleReceivedPacket(const uint32_t wait_usec);
    void receiveData(const uint8_t* data);

    int SendExecuteTrigger();
    
    int ReceiveReturnCode(unsigned char intf);
    

  private:
    std::string GetStringFromPacket(const unsigned char* start_adr, int length);
  };
}
