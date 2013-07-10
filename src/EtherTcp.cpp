#include "EtherTcp.h"
#include <iostream>


#define BUFSIZE 256
uint8_t buffer[BUFSIZE];
int32_t buffer_start = 0;
int32_t buffer_end   = 0;

void buffer_push(uint8_t dat) {
	buffer[buffer_end] = dat;
	buffer_end++;
	if(buffer_end >= BUFSIZE) {
		buffer_end = 0;
	}
}

uint8_t buffer_pop() {
	uint8_t dat = buffer[buffer_start];
	buffer_start++;
	if(buffer_start >= BUFSIZE) {
		buffer_start = 0;
	}
	return dat;
}

int32_t get_buffer_size() {
	int32_t size = buffer_end - buffer_start;
	if(size < 0) {
		size += BUFSIZE;
	}
	return size;
}

using namespace ssr;

EtherTcp::EtherTcp(const char* ipAddress, int32_t port)
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

void EtherTcp::flushRxBuffer()
{

}

/**
 * @brief flush transmit buffer.
 * @return zero if success.
 */
void EtherTcp::flushTxBuffer()
{

}

/**
 * @brief Get stored datasize of in Rx Buffer
 * @return Stored Data Size of Rx Buffer;
 */
RETVAL EtherTcp::getSizeInRxBuffer()
{
/*
	fd_set read_fdset;
	FD_ZERO(&read_fdset);
	FD_SET(m_ServerSocket, &read_fdset);

	select(0, &read_fdset, NULL, NULL, NULL);

	if(FD_ISSET(m_ServerSocket, &read_fdset)) {
		uint8_t buf[BUFSIZE];
		RETVAL readable_size = BUFSIZE - get_buffer_size();
		RETVAL read_size = recv(m_ServerSocket, (char*)buf, readable_size, 0);
		for(RETVAL i = 0;i < read_size;i++) {
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
RETVAL EtherTcp::write(const uint8_t* src, const uint8_t size)
{
	return m_pSocket->send(src, size);
}

/**
 * @brief read data from RxBuffer of Serial Port 
 */
RETVAL EtherTcp::read(uint8_t *dst, const uint8_t size)
{
	//return recv(m_ServerSocket, (char*)dst, size, 0);
	uint8_t *dist_buf = (uint8_t*)dst;
	uint32_t size_read =  size;
	if(size >= (uint32_t)get_buffer_size()) {
		size_read = get_buffer_size();
	}
	for(uint32_t i = 0;i < size_read;i++) {
		dist_buf[i] = buffer_pop();
	}
		
	return size_read;
}


RETVAL EtherTcp::svc(void)
{
	m_Endflag = 0;
	m_pSocket->setNonBlock(1);
	while(!m_Endflag) {
		uint8_t buf;
		int32_t sz = m_pSocket->recv(&buf, 1);
		if(sz == 1) {
			buffer_push(buf);
		}
	}
	return 0;
}


RETVAL EtherTcp::getSenderInfo(uint8_t *buffer)
{
  buffer[0] = 'E', buffer[1] = 'T', buffer[2] = 'C', buffer[3] = 'P';
  return 0;
}
