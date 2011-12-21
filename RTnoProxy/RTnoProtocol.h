#pragma once

#include "Transport.h"
#include "RTnoProfile.h"



namespace org {
	namespace ysuga {
		class RTnoRTObjectWrapper;

		class RTnoProtocol
		{
		private:
			int m_SendBusy;
			Transport *m_pTransport;
			RTnoRTObjectWrapper *m_pRTObjectWrapper;
		public:
			RTnoProtocol(RTnoRTObjectWrapper *pRTObject, Transport* pTransport);
			virtual ~RTnoProtocol(void);

		public:
			int GetRTnoProfile(RTnoProfile* profile);
			unsigned char GetRTnoStatus();
			unsigned char GetRTnoExecutionContextType();

			int ActivateRTno();
			int DeactivateRTno();

			int SendData(const char* portName, const unsigned char* data, int length);
			int ReceiveData(unsigned char* packet_buffer);
			int SendExecuteTrigger();

			int ReceiveReturnCode(unsigned char intf);

			int HandleReceivedPacket(void);
		private:
			std::string GetStringFromPacket(const unsigned char* start_adr, int length);
		};

	};
};