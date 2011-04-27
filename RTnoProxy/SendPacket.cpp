#include <iostream>
#include "SendPacket.h"


int SendPacket(org::ysuga::SerialPort* pSerialPort, const char arg_interface, const char data_length, const char* packet_data) {
  char sum = 0;
  //  std::cout << "SendPacket(" << interface << "L=" << (int)data_length << ")" << std::endl;
  pSerialPort->Write(&arg_interface, 1);
  sum += arg_interface;
  pSerialPort->Write(&data_length, 1);
  sum += data_length;
  for(int i = 0;i < data_length;i++) {
    sum += packet_data[i];
  }
  if(data_length != 0) {
    pSerialPort->Write(packet_data, data_length);
  }
  pSerialPort->Write(&sum, 1);
  return PACKET_HEADER_SIZE + data_length + 1;
}
