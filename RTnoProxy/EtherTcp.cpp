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
	m_pSocket = new net::ysuga::Socket(ipAddress, port);
	coil::Task::activate();
}


EtherTcp::~EtherTcp()
{
	m_Endflag = 1;
	coil::Task::wait();
	delete m_pSocket;
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
	return m_pSocket->send((const char*)src, size);
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


int EtherTcp::svc(void)
{
	m_Endflag = 0;
	m_pSocket->setNonBlock(1);
	while(!m_Endflag) {
		char buf;
		int sz = m_pSocket->recv(&buf, 1);
		if(sz == 1) {
			buffer_push(buf);
		}
	}
	return 0;
}