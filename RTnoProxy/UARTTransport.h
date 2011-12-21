#pragma once
#include "Transport.h"


#include "SerialPort.h"

namespace org {
	namespace ysuga {
		class UARTTransport : public org::ysuga::Transport
		{
		public:

			UARTTransport(const char* filename, const int baudrate)
			{
				m_pSerialDevice = new SerialPort(filename, baudrate);
			}

			virtual ~UARTTransport(void)
			{
				delete m_pSerialDevice;
			}
		};

	};
};
