#pragma once

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

			int ReceivePacket(unsigned char* packet);

			int IsReceived();
		private:
			int _Wait(unsigned char buffer_size);
		};
	};
};
