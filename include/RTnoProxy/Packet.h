
#ifndef PACKET_HEADER_INCLUDED
#define PACKET_HEADER_INCLUDED

// Return Values
#define TIMEOUT 1
#define DATA_TIMEOUT 2
#define CHECKSUM_ERROR 3

#define INVALID_PACKET_INTERFACE 65
#define INVALID_PACKET_DATASIZE  66

// Packet Settings
#define PACKET_HEADER_SIZE 2
#define PACKET_INTERFACE 0
#define DATA_LENGTH 1
#define DATA_START_ADDR 2

// Protocol
// Interface
#define INITIALIZE 'I'
#define ACTIVATE 'A'
#define DEACTIVATE 'D'
#define EXECUTE 'E'
#define ONERROR 'C'
#define RTNO_RESET 'R'
#define GET_STATUS 'X'
#define SEND_DATA 'S'
#define RECEIVE_DATA 'V'
#define GET_PROFILE 'Z'
#define GET_CONTEXT 'B'


#define ADD_INPORT 'P'
#define ADD_OUTPORT 'Q'

#define INPORT_ISNEW 'N'
#define INPORT_READ  'J'

#define RTNO_OK '@'
#define RTNO_ERROR 'x'


#define OUTPORT_WRITE 'W'

// Communication Settings
#define PACKET_WAITING_TIME 3000 // ms
#define PACKET_SENDING_DELAY 10 // us
#define PACKET_WAITING_DELAY 100 //us
#define PACKET_WAITING_COUNT (PACKET_WAITING_TIME*1000/PACKET_WAITING_DELAY)

#define TYPECODE_TIMED_BOOLEAN 'b'
#define TYPECODE_TIMED_OCTET 'o'
#define TYPECODE_TIMED_CHAR  'c'

#define TYPECODE_TIMED_LONG 'l'
#define TYPECODE_TIMED_FLOAT 'f'
#define TYPECODE_TIMED_DOUBLE 'd'

#define TYPECODE_TIMED_BOOLEAN_SEQ 'B'
#define TYPECODE_TIMED_OCTET_SEQ 'O'
#define TYPECODE_TIMED_CHAR_SEQ 'C'

#define TYPECODE_TIMED_LONG_SEQ 'L'
#define TYPECODE_TIMED_FLOAT_SEQ 'F'
#define TYPECODE_TIMED_DOUBLE_SEQ 'D'

#define MAX_PACKET_SIZE 64

/***********************************************
 *
 **********************************************/
#define ConnectionTypeSerial1 0x01
#define ConnectionTypeSerial2 0x02
#define ConnectionTypeSerial3 0x03

#define ProxySynchronousExecutionContext 0x21
#define Timer1ExecutionContext 0x22
#define Timer2ExecutionContext 0x23


#endif