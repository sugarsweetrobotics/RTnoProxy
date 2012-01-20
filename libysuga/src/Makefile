TARGET=../lib/libysuga.a

all: $(TARGET)


CC=g++
AR=ar
CFLAGS=-O2 -Wall -fPIC -I../include -c 
ARFLAGS=rv

OBJECTS=SerialPort.o Socket.o Thread.o Mutex.o 

.cpp.o:
	$(CC) $(CFLAGS) $<


../lib/libysuga.a: $(OBJECTS)
	$(AR) $(ARFLAGS) ../lib/libysuga.a $(OBJECTS)


clean:
	rm -rf *~ *.o $(TARGET)

