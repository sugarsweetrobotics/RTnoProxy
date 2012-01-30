TARGET=lib/libysuga.a



CFLAGS=-Wall -O2 
LDFLAGS=-Wall -O2

all: $(TARGET)


$(TARGET):
	cd src;make;

.cpp.o:
	$(CC) $(CFLAGS) -c $<


clean:
	cd src; make clean;