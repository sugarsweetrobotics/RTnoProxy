#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include "RTnoPacket.h"


using namespace ssr;


void RTnoPacket::dump()
{
  for(uint8_t i = 0;i < getPacketLength();i++) {
    std::cerr << std::hex << std::showbase << std::setw(2) << std::setfill('0') << (int)m_pData[i] << "(";
    std::cerr << (isascii(m_pData[i]) ? (char)m_pData[i] : ' ')  << ")" << " ";
    if (i % 4 == 0 && i != 0 && i % 8 != 0) {
      std::cerr << " ";
    } else if (i % 8 == 0 && i!=0) {
      std::cerr << std::endl;
    }
  }
}
