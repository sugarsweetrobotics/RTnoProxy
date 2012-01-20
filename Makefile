all: bin/RTnoProxyComp bin/DataPortTestComp


bin/RTnoProxyComp:
	cd RTnoProxy; ${MAKE}

bin/DataPortTestComp:
	cd DataPortTest; ${MAKE}

clean:
	cd RTnoProxy; ${MAKE} clean;
	cd DataPortTest; ${MAKE} clean;
	cd libysuga; ${MAKE} clean;
	rm -rf bin/RTnoProxyComp bin/DataPortTestComp *~