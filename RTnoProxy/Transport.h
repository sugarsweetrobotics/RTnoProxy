#pragma once

#include "SerialDevice.h"

namespace org {
	namespace ysuga { 
		class Transport
		{
		private:

		protected:
			SerialDevice *m_pSerialDevice;

		public:
			Transport(void);
			~Transport(void);


		public:
			int SendPacket(unsigned char interFace, unsigned char size, unsigned char* packet_buffer);

			int ReceivePacket(unsigned char* packet);

		};
	};
};
