#include <iostream>
#include <coil/Time.h>
#include "SendPacket.h"


int SendPacket(org::ysuga::SerialPort* pSerialPort, const char arg_interface, const char data_length, const unsigned char* packet_data) {
  char sum = 0;
  //  std::cout << "SendPacket(" << interface << "L=" << (int)data_length << ")" << std::endl;
  pSerialPort->Write(&arg_interface, 1);
  sum += arg_interface;
  coil::usleep(PACKET_SENDING_DELAY);
  pSerialPort->Write(&data_length, 1);
  sum += data_length;
  coil::usleep(PACKET_SENDING_DELAY);
  for(int i = 0;i < data_length;i++) {
    sum += packet_data[i];
    pSerialPort->Write(packet_data+i, 1);
	coil::usleep(PACKET_SENDING_DELAY);
  }
  pSerialPort->Write(&sum, 1);
  return PACKET_HEADER_SIZE + data_length + 1;
}
