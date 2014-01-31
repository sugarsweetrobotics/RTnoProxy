/********************************************************
 * SerialPort.h
 *
 * Portable Serial Port Class Library for Windows and Unix.
 * @author ysuga@ysuga.net
 * @date 2010/11/02
 ********************************************************/

#ifndef SERIAL_PORT_HEADER_INCLUDED
#define SERIAL_PORT_HEADER_INCLUDED

#include "libysuga.h"

#include <exception>
#include <string>

#ifdef WIN32
#include <windows.h>
#endif

namespace net {
	namespace ysuga {

		/**
		 * Base Class for Exception
		 */
		class LIBYSUGA_API ComException : public std::exception {
		private:
			std::string msg;
		public:
			ComException(const char* msg) {this->msg = msg;}
			virtual ~ComException() throw() {}
		public:
			const char* what() const throw() {return msg.c_str();}
		};

		/**
		 * @brief  This exception is thrown when COM port accessing is wrong.
		 */
		class LIBYSUGA_API ComAccessException : public ComException {
		public:
			ComAccessException(void) : ComException("COM Access") {}
			virtual ~ComAccessException(void) throw() {}
		};

		/**
		 * @brief  This exception is thrown when COM port state is wrong.
		 */
		class LIBYSUGA_API ComStateException : public ComException {
		public:
			ComStateException(void) : ComException ("COM State Exception") {}
			virtual ~ComStateException(void) throw() {}
		};
    
		/**
		 * @brief This exception is thrown when Opening COM port is failed.
		 */
		class LIBYSUGA_API ComOpenException : public ComException  {
		public:
			ComOpenException(void) : ComException ("COM Open Error") {}
			virtual ~ComOpenException(void) throw() {}
		};




		/***************************************************
		 * SerialPort
		 *
		 * @brief Portable Serial Port Class
		 ***************************************************/
		class LIBYSUGA_API SerialPort
		{
		private:
#ifdef WIN32

			HANDLE m_hComm;
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
			 * @param filename Filename of Serial Port (eg., "COM0", "/dev/tty0")
			 * @baudrate baudrate. (eg., 9600, 115200)
			 */
			SerialPort(const char* filename, int baudrate);

			/**
			 * @brief Destructor
			 */
			~SerialPort();

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
			int getSizeInRxBuffer();

			/**
			 * @brief write data to Tx Buffer of Serial Port.
			 *
			 */
			int write(const void* src, const unsigned int size);

			/**
			 * @brief read data from RxBuffer of Serial Port 
			 			 */
			int read(void *dst, const unsigned int size);

		};

	};//namespace ysuga
};//namespace org

#endif

/*******************************************************
 * Copyright  2010, ysuga.net all rights reserved.
 *******************************************************/
