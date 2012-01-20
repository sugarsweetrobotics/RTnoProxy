#include "Socket.h"

using namespace net::ysuga;

Socket::Socket(const char* ipAddress, unsigned short port)
{
	m_Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_Socket == INVALID_SOCKET) {
		throw SocketCreateException();
	}
	m_SocketAddr.sin_family = AF_INET;
	m_SocketAddr.sin_port = htons(port);
	m_SocketAddr.sin_addr.S_un.S_addr = inet_addr(ipAddress);

	if(connect(m_Socket, (struct sockaddr *)&m_SocketAddr, sizeof(m_SocketAddr)) != 0) {
		throw SocketConnectException();
	}
}

Socket::~Socket(void)
{
	closesocket(m_Socket);
}


int Socket::setNonBlock(unsigned int flag)
{
	u_long val =  flag;
	return ::ioctlsocket(m_Socket, FIONBIO, &val);
}

int Socket::recv(char* buf, int len)
{
	return ::recv(m_Socket, buf, len, 0);
}

int Socket::send(const char* buf, int len)
{
	return ::send(m_Socket, buf, len, 0);
}

