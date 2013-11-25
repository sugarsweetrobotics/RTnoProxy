#include "Socket.h"
#ifdef WIN32


#else
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#endif

using namespace net::ysuga;

Socket::Socket(const char* ipAddress, unsigned short port)
{

	m_Socket = socket(AF_INET, SOCK_STREAM, 0);
#ifdef WIN32
	if (m_Socket == INVALID_SOCKET) 
	  throw SocketCreateException();
#else 
	if( m_Socket < 0)
	  throw SocketCreateException();
#endif

	m_SocketAddr.sin_family = AF_INET;
	m_SocketAddr.sin_port = htons(port);
#ifdef WIN32
	m_SocketAddr.sin_addr.S_un.S_addr = inet_addr(ipAddress);
#else
	m_SocketAddr.sin_addr.s_addr = inet_addr(ipAddress);
#endif

	if(connect(m_Socket, (struct sockaddr *)&m_SocketAddr, sizeof(m_SocketAddr)) != 0) {
		throw SocketConnectException();
	}
}

Socket::~Socket(void)
{
#ifdef WIN32
	closesocket(m_Socket);
#else
	close(m_Socket);
#endif
}


int Socket::setNonBlock(unsigned int flag)
{
	u_long val =  flag;
#ifdef WIN32
	return ::ioctlsocket(m_Socket, FIONBIO, &val);
#else
	return ioctl(m_Socket, FIONBIO, &val);
#endif
}

int Socket::recv(uint8_t* buf, uint32_t len)
{
  return ::recv(m_Socket, (char*)buf, len, 0);
}

int Socket::send(const uint8_t* buf, uint32_t len)
{
  return  ::send(m_Socket, (char*)buf, len, 0);
}

