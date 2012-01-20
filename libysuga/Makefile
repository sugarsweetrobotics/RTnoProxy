TARGET=lib/libysuga.a

CC=g++
CFLAGS=-Wall -O2 
LDFLAGS=-Wall -O2

all: $(TARGET)


$(TARGET):
	cd src;make;

.cpp.o:
	$(CC) $(CFLAGS) -c $<


clean:
	cd src; make clean;