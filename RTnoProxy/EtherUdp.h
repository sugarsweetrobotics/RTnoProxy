#ifndef ETHER_UDP_HEADER_INCLUDED
#define ETHER_UDP_HEADER_INCLUDED

#include <exception>

#include <coil/Task.h>
#include <string>
#include <queue>

#include "SerialDevice.h"
#include "DatagramSocket.h"


namespace net {
  namespace ysuga {
    
    /***************************************************
     * SerialPort
     *
     * @brief Portable Serial Port Class
     ***************************************************/

    class EtherUdp : public SerialDevice, public coil::Task 
      {
    private:
      int m_Endflag;
      std::string m_ipAddress;
      uint16_t m_Port;
      net::ysuga::DatagramSocket* m_pSocket;
      net::ysuga::DatagramSocket* m_pServerSocket;
      net::ysuga::DatagramPacket* m_pPacket;
      uint16_t m_DataPointer;
      std::queue<net::ysuga::DatagramPacket*> m_Queue;
    public:
      /**
       * @brief Constructor
       * 
       * @param ipaddress Ip Address of target device
       * @param port port number.
       */
      EtherUdp(const char* ipadderss, int port);
      
      /**
       * @brief Destructor
       */
      virtual ~EtherUdp();
      
    public:
      virtual void ReceivePacket();
  
      /**
       * @brief flush receive buffer.
       * @return zero if success.
       */
      void FlushRxBuffer();
      
      /**
       * @brief flush transmit buffer.
       * @return zero if success.
       */
      void FlushTxBuffer();
      
    public:
      /**
       * @brief Get stored datasize of in Rx Buffer
       * @return Stored Data Size of Rx Buffer;
       */
      int GetSizeInRxBuffer();
      
      /**
       * @brief write data to Tx Buffer of Serial Port.
       *
       */
      int Write(const void* src, const unsigned int size);
      
      /**
       * @brief read data from RxBuffer of Serial Port 
       */
      int Read(void *dst, const unsigned int size);
      
    public:
      virtual int svc(void);
    };
    
  }//namespace ysuga
}//namespace org

#endif

/*******************************************************
 * Copyright  2010, ysuga.net all rights reserved.
 *******************************************************/
