#pragma once

#include <string.h>
#include <list>


namespace net {
	namespace ysuga {

		class PortProfile 
		{
		private:
			unsigned char m_TypeCode;
			char* m_pPortName;
		public:
			PortProfile(unsigned char typeCode, const char* portName) {
				m_TypeCode = typeCode;
				m_pPortName = new char[strlen(portName)];
				strcpy(m_pPortName, portName);
			}

			virtual ~PortProfile() {
				delete[] m_pPortName;
			}

			unsigned char GetTypeCode() {
				return m_TypeCode;
			}

			const char* GetPortName() {
				return m_pPortName;
			}
		};

		typedef std::list<PortProfile*> PortList;
		typedef std::list<PortProfile*>::iterator PortListIterator;


		class RTnoProfile
		{
		private:
			PortList m_InPortProfileList;
			PortList m_OutPortProfileList;
		public:

			RTnoProfile(void)
			{
			}

			virtual ~RTnoProfile(void)
			{
				for(std::list<PortProfile*>::iterator it = m_InPortProfileList.begin(); it != m_InPortProfileList.end(); ++it) {
					delete (*it);
				}
				m_InPortProfileList.clear();

				for(std::list<PortProfile*>::iterator it = m_OutPortProfileList.begin(); it != m_OutPortProfileList.end(); ++it) {
					delete (*it);
				}
				m_OutPortProfileList.clear();
			}

		public:
			
			void AddInPort(unsigned char typeCode, const char* portName) {
				m_InPortProfileList.push_back(new PortProfile(typeCode, portName));
			}

			void AddOutPort(unsigned char typeCode, const char* portName) {
				m_OutPortProfileList.push_back(new PortProfile(typeCode, portName));
			}

			PortList* GetInPortList() {
				return &m_InPortProfileList;
			}

			PortList* GetOutPortList() {
				return &m_OutPortProfileList;
			}

		        uint8_t GetInPortIndex(const char* portName) {
			        uint8_t i = 0;
				for(std::list<PortProfile*>::iterator it = m_InPortProfileList.begin(); it != m_InPortProfileList.end(); ++it, i++) {
					if(strcmp((*it)->GetPortName(), portName) == 0) {
						return i;
					}
				}
			  
			}

			uint8_t GetOutPortIndex(const char* portName) {
      			        uint8_t i = 0;
			        for(std::list<PortProfile*>::iterator it = m_OutPortProfileList.begin(); it != m_OutPortProfileList.end(); ++it, i++) {
					if(strcmp((*it)->GetPortName(), portName) == 0) {
						return i;
					}
				}
			}

			void RemoveInPort(const char* portName) {
				for(std::list<PortProfile*>::iterator it = m_InPortProfileList.begin(); it != m_InPortProfileList.end(); ++it) {
					if(strcmp((*it)->GetPortName(), portName) == 0) {
						m_InPortProfileList.remove((*it));
						return;
					}
				}
			}

			void RemoveOutPort(const char* portName) {
				for(std::list<PortProfile*>::iterator it = m_OutPortProfileList.begin(); it != m_OutPortProfileList.end(); ++it) {
					if(strcmp((*it)->GetPortName(), portName) == 0) {
						m_OutPortProfileList.remove((*it));
						return;
					}
				}
			}

		};


	};
};
