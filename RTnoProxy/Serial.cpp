#include "Serial.h"

using namespace net::ysuga;

Serial::Serial(const char* portName, const int boadrate)
{
	m_pSerialPort = new net::ysuga::SerialPort(portName, boadrate);
}

Serial::~Serial(void)
{
	delete m_pSerialPort;
}
