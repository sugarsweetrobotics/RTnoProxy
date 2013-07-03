#pragma once


#include <stdint.h>

namespace ssr {
  
  typedef int32_t RETVAL;

  class SerialDevice {
  public:
    
    SerialDevice(void) {
    }
    
    virtual ~SerialDevice(void) {
    }
    
  public:
    /**
     * @brief flush receive buffer.
     * @return zero if success.
     */
    virtual void flushRxBuffer() = 0;
    
    /**
     * @brief flush transmit buffer.
     * @return zero if success.
     */
    virtual void flushTxBuffer() = 0;
    
  public:
    /**
     * @brief Get stored datasize of in Rx Buffer
     * @return Stored Data Size of Rx Buffer;
     */
    virtual RETVAL getSizeInRxBuffer() = 0;
    
    /**
     * @brief write data to Tx Buffer of Serial Port.
     *
     */
    virtual RETVAL write(const uint8_t* src, const uint8_t size)  = 0;
    
    /**
     * @brief read data from RxBuffer of Serial Port 
     */
    virtual RETVAL read(uint8_t *dst, const uint8_t size) = 0;
    
    /**
     */
    virtual RETVAL getSenderInfo(uint8_t* buffer) = 0;
  };
};
