#ifndef ETHER_TCP_HEADER_INCLUDED
#define ETHER_TCP_HEADER_INCLUDED

#include <exception>

#include <coil/Task.h>


#include "SerialDevice.h"
#include "Socket.h"


namespace org {
	namespace ysuga {

		/***************************************************
		 * SerialPort
		 *
		 * @brief Portable Serial Port Class
		 ***************************************************/
		class EtherTcp : public SerialDevice, public coil::Task
		{
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

	};//namespace ysuga
};//namespace org

#endif

/*******************************************************
 * Copyright  2010, ysuga.net all rights reserved.
 *******************************************************/
