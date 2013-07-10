#pragma once

// Return Values
#define TIMEOUT 1
#define DATA_TIMEOUT 2
#define CHECKSUM_ERROR 3
#define INVALID_PACKET_INTERFACE 65
#define INVALID_PACKET_DATASIZE  66

// Protocol
// Interface
#define INITIALIZE 'I'
#define RTNO_ACTIVATE 'A'
#define RTNO_DEACTIVATE 'D'
#define RTNO_EXECUTE 'E'
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

#define PACKET_ERROR 'F'

#define OUTPORT_WRITE 'W'

// state
enum {
  STATE_CREATED='C',
  STATE_INACTIVE='I',
  STATE_ACTIVE='A',
  STATE_ERROR='E',
  STATE_NONE='N',
};



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

