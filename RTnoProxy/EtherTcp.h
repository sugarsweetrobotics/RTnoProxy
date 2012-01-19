#ifndef ETHER_TCP_HEADER_INCLUDED
#define ETHER_TCP_HEADER_INCLUDED

#include <exception>

#include "SerialDevice.h"
#include "Thread.h"

#ifdef WIN32
#include <windows.h>
#endif


namespace org {
	namespace ysuga {

		/***************************************************
		 * SerialPort
		 *
		 * @brief Portable Serial Port Class
		 ***************************************************/
		class EtherTcp : public SerialDevice, public net::ysuga::Thread
		{
		private:
			int m_Endflag;
#ifdef WIN32
			SOCKET m_ServerSocket;
			struct sockaddr_in m_SocketAddr;
#else

			/**
			 * @brief file descriptor
			 */
			int m_Fd;
#endif



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
			void Run(void);
		};

	};//namespace ysuga
};//namespace org

#endif

/*******************************************************
 * Copyright  2010, ysuga.net all rights reserved.
 *******************************************************/
