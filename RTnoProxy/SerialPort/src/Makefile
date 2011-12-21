TARGET=../lib/SerialPort.a

all: $(TARGET)


CC=g++
AR=ar
CFLAGS=-O2 -Wall -fPIC -I../include -c 
ARFLAGS=urv

OBJECTS=SerialPort.o

.cpp.o:
	$(CC) $(CFLAGS) $<


../lib/SerialPort.a: $(OBJECTS)
	$(AR) $(ARFLAGS) ../lib/SerialPort.a $(OBJECTS)


clean:
	rm -rf *~ *.o $(TARGET)

