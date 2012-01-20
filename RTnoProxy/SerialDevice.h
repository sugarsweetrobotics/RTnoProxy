#pragma once


namespace net {
	namespace ysuga {

		class SerialDevice
		{
		public:

			SerialDevice(void)
			{
			}

			virtual ~SerialDevice(void)
			{
			}

		public:
			/**
			 * @brief flush receive buffer.
			 * @return zero if success.
			 */
			virtual void FlushRxBuffer() = 0;

			/**
			 * @brief flush transmit buffer.
			 * @return zero if success.
			 */
			virtual void FlushTxBuffer() = 0;

		public:
			/**
			 * @brief Get stored datasize of in Rx Buffer
			 * @return Stored Data Size of Rx Buffer;
			 */
			virtual int GetSizeInRxBuffer() = 0;

			/**
			 * @brief write data to Tx Buffer of Serial Port.
			 *
			 */
			virtual int Write(const void* src, const unsigned int size)  = 0;

			/**
			 * @brief read data from RxBuffer of Serial Port 
			 			 */
			virtual int Read(void *dst, const unsigned int size) = 0;
		};
	};
};
