#pragma once

#include <stdint.h>
#include "SerialDevice.h"

namespace net {
	namespace ysuga { 
		class Transport
		{
		private:

		protected:
			SerialDevice *m_pSerialDevice;

		public:
			Transport(SerialDevice *pSerialDevice);
			~Transport(void);


		public:
			int SendPacket(unsigned char interFace, unsigned char size, unsigned char* packet_buffer);

			int ReceivePacket(uint8_t* packet);

			int IsReceived();
		private:
			int _Wait(unsigned char buffer_size);
		};
	};
};
