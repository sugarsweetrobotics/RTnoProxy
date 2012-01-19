#pragma once
#include "Transport.h"


#include "EtherTcp.h"

namespace org {
	namespace ysuga {
		class EtherTcpTransport : public org::ysuga::Transport
		{
		public:

			EtherTcpTransport(const char* ipaddress, const int port)
			{
				m_pSerialDevice = new EtherTcp(ipaddress, port);
			}

			virtual ~EtherTcpTransport(void)
			{
				delete m_pSerialDevice;
			}
		};

	};
};
