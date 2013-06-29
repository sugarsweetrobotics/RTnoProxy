#pragma once

#include <stdint.h>
#include "SerialDevice.h"


extern const uint32_t INFINITE;
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

			int32_t ReceivePacket(uint8_t* packet, const uint32_t wait_usec=INFINITE);

			int IsReceived();
		private:
			inline int32_t read(uint8_t *buffer, uint8_t size, const uint32_t wait_usec = INFINITE);
			inline int32_t write(const uint8_t *buffer, const uint8_t size);
			int _Wait(unsigned char buffer_size);
		};
	};
};
