#include "EtherTcp.h"
#include <iostream>


#define BUFSIZE 256
unsigned char buffer[BUFSIZE];
int buffer_start = 0;
int buffer_end   = 0;

void buffer_push(unsigned char dat) {
	buffer[buffer_end] = dat;
	buffer_end++;
	if(buffer_end >= BUFSIZE) {
		buffer_end = 0;
	}
}

unsigned char buffer_pop() {
	unsigned char dat = buffer[buffer_start];
	buffer_start++;
	if(buffer_start >= BUFSIZE) {
		buffer_start = 0;
	}
	return dat;
}

int get_buffer_size() {
	int size = buffer_end - buffer_start;
	if(size < 0) {
		size += BUFSIZE;
	}
	return size;
}

using namespace org::ysuga;

EtherTcp::EtherTcp(const char* ipAddress, int port)
{
	m_ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_ServerSocket == INVALID_SOCKET) {
		std::cout << "--- Socket Create Error." << std::endl;
		return;
	}
	m_SocketAddr.sin_family = AF_INET;
	m_SocketAddr.sin_port = htons(port);
	m_SocketAddr.sin_addr.S_un.S_addr = inet_addr(ipAddress);

	if(connect(m_ServerSocket, (struct sockaddr *)&m_SocketAddr, sizeof(m_SocketAddr)) != 0) {
		std::cout << "--- Socket Connect Error." << std::endl;
		return;
	}
	Start();
}


EtherTcp::~EtherTcp()
{
	m_Endflag = 1;
	Join();
	closesocket(m_ServerSocket);
}

void EtherTcp::FlushRxBuffer()
{

}

/**
 * @brief flush transmit buffer.
 * @return zero if success.
 */
void EtherTcp::FlushTxBuffer()
{

}

/**
 * @brief Get stored datasize of in Rx Buffer
 * @return Stored Data Size of Rx Buffer;
 */
int EtherTcp::GetSizeInRxBuffer()
{
/*
	fd_set read_fdset;
	FD_ZERO(&read_fdset);
	FD_SET(m_ServerSocket, &read_fdset);

	select(0, &read_fdset, NULL, NULL, NULL);

	if(FD_ISSET(m_ServerSocket, &read_fdset)) {
		unsigned char buf[BUFSIZE];
		int readable_size = BUFSIZE - get_buffer_size();
		int read_size = recv(m_ServerSocket, (char*)buf, readable_size, 0);
		for(int i = 0;i < read_size;i++) {
			buffer_push(buf[i]);
		}
	}

	return get_buffer_size();;
*/
//	return 256;

	return ::get_buffer_size();
}

/**
 * @brief write data to Tx Buffer of Serial Port.
 *
 */
int EtherTcp::Write(const void* src, const unsigned int size)
{
	return send(m_ServerSocket, (const char*)src, size, 0);
}

/**
 * @brief read data from RxBuffer of Serial Port 
 			 */
int EtherTcp::Read(void *dst, const unsigned int size)
{
	//return recv(m_ServerSocket, (char*)dst, size, 0);
	unsigned char *dist_buf = (unsigned char*)dst;
	unsigned int size_read = size;
	if(size >= get_buffer_size()) {
		size_read = get_buffer_size();
	}
	for(int i = 0;i < size_read;i++) {
		dist_buf[i] = buffer_pop();
	}
		
	return size_read;
}


void EtherTcp::Run(void)
{
	m_Endflag = 0;
	u_long flag = 1;
	::ioctlsocket(m_ServerSocket, FIONBIO, &flag);
	while(!m_Endflag) {
		char buf;
		int sz = recv(m_ServerSocket, &buf, 1, 0);
		if(sz == 1) {
			buffer_push(buf);
		}
	}
}