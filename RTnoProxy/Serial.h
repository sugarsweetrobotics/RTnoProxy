#pragma once

#include "SerialDevice.h"
#include "SerialPort.h"

namespace net {
	namespace ysuga { 
		class Serial : public SerialDevice
		{
		private:
			SerialPort *m_pSerialPort;
		public:
			Serial(const char* portName, const int boardrate);
			virtual ~Serial(void);

		public:
			/**
			 * @brief flush receive buffer.
			 * @return zero if success.
			 */
			virtual void FlushRxBuffer() {
				m_pSerialPort->flushRxBuffer();
			}

			/**
			 * @brief flush transmit buffer.
			 * @return zero if success.
			 */
			virtual void FlushTxBuffer() {
				m_pSerialPort->flushTxBuffer();
			}

		public:
			/**
			 * @brief Get stored datasize of in Rx Buffer
			 * @return Stored Data Size of Rx Buffer;
			 */
			virtual int GetSizeInRxBuffer() {
				return m_pSerialPort->getSizeInRxBuffer();
			}

			/**
			 * @brief write data to Tx Buffer of Serial Port.
			 *
			 */
			virtual int Write(const void* src, const unsigned int size) {
				return m_pSerialPort->write(src, size);
			}

			/**
			 * @brief read data from RxBuffer of Serial Port 
			 			 */
			virtual int Read(void *dst, const unsigned int size) {
				return m_pSerialPort->read(dst, size);
			}
		};
	}
}
