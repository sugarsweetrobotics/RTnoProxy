

all: bin/RTnoProxyComp bin/DataPortTestComp

CC=/opt/local/bin/gcc-apple-4.2
CXX=/opt/local/bin/g++-apple-4.2

bin/RTnoProxyComp: libysuga/lib/libysuga.a
	cd RTnoProxy; ${MAKE}

libysuga/lib/libysuga.a:
	cd libysuga; ${MAKE}

bin/DataPortTestComp:
	cd DataPortTest; ${MAKE}

clean:
	cd RTnoProxy; ${MAKE} clean;
	cd DataPortTest; ${MAKE} clean;
	cd libysuga; ${MAKE} clean;
	rm -rf bin/RTnoProxyComp bin/DataPortTestComp *~