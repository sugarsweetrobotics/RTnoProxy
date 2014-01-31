#pragma once

#include "libysuga.h"

#include <stdint.h>
#ifdef WIN32
#include <windows.h>

#else
#include <sys/socket.h>
#include <netinet/in.h>

#endif

#include <string>
#include <exception>

namespace net {
	namespace ysuga {
		class LIBYSUGA_API SocketException : public std::exception {
		private:
			std::string msg;
		public:
			SocketException(const char* msg) {this->msg = msg;}
			virtual ~SocketException() throw() {}
		public:
			const char* what() const  throw() {return msg.c_str();}
		};


		class LIBYSUGA_API SocketCreateException : public SocketException {
		public:
			SocketCreateException(void) : SocketException("SocketCreate Error") {}
			virtual ~SocketCreateException(void) throw() {}
		};


		class LIBYSUGA_API SocketConnectException : public SocketException {
		public:
			SocketConnectException(void) : SocketException("SocketConnect Error") {}
			virtual ~SocketConnectException(void) throw() {}
		};

		class LIBYSUGA_API Socket
		{
		private:
		#ifdef WIN32
			SOCKET m_Socket;
			struct sockaddr_in m_SocketAddr;
		#else

			/**
			 * @brief file descriptor
			 */
			int m_Socket;
			struct sockaddr_in m_SocketAddr;
		#endif

		public:
			Socket(const char* ipAddress, unsigned short port);
			~Socket(void);


		public:
			int setNonBlock(unsigned int flag);
			int recv(uint8_t* buf, uint32_t len);
			int send(const uint8_t* buf, uint32_t len);
		};

	};
};
