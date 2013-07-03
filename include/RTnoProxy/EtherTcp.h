#pragma once

#include <exception>
#include <coil/Task.h>

#include "SerialDevice.h"
#include "Socket.h"


namespace ssr {
  
  /***************************************************
   * SerialPort
   *
   * @brief Portable Serial Port Class
   ***************************************************/
  class EtherTcp : public SerialDevice, public coil::Task {
  private:
    int m_Endflag;
    net::ysuga::Socket *m_pSocket;
    
    
  public:
    /**
     * @brief Constructor
     * 
     * @param ipaddress Ip Address of target device
     * @param port port number.
     */
    EtherTcp(const char* ipadderss, int port);
    
    /**
     * @brief Destructor
     */
    virtual ~EtherTcp();
    
  public:
    /**
     * @brief flush receive buffer.
     * @return zero if success.
     */
    void flushRxBuffer();
    
    /**
     * @brief flush transmit buffer.
     * @return zero if success.
     */
    void flushTxBuffer();
    
  public:
    /**
     * @brief Get stored datasize of in Rx Buffer
     * @return Stored Data Size of Rx Buffer;
     */
    RETVAL getSizeInRxBuffer();

    /**
     * @brief write data to Tx Buffer of Serial Port.
     *
     */
    RETVAL write(const uint8_t* src, const uint8_t size);

    /**
     * @brief read data from RxBuffer of Serial Port 
     */
    RETVAL read(uint8_t *dst, const uint8_t size);

    RETVAL getSenderInfo(uint8_t *buffer);
    
  public:
    virtual int svc(void);
  };
  
};//namespace ssr



/*******************************************************
 * Copyright  2010, ysuga.net all rights reserved.
 *******************************************************/
