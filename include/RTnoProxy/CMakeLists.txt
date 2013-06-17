set(hdrs RTnoProxy.h EtherTcp.h InPortWrapper.h OutPortWrapper.h Mutex.h Packet.h RTnoProfile.h RTnoProtocol.h RTnoRTObjectWrapper.h Serial.h SerialPort.h SerialDevice.h ServerSocket.h Socket.h Thread.h Transport.h libysuga.h 
    PARENT_SCOPE
    )

install(FILES ${hdrs} DESTINATION ${INC_INSTALL_DIR}/${PROJECT_NAME_LOWER}
    COMPONENT library)

