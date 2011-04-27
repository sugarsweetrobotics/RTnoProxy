// -*- C++ -*-
/*!
 * @file  DataPortTest.cpp
 * @brief DataPortTest
 * @date $Date$
 *
 * $Id$
 */

#include "DataPortTest.h"
#ifdef WIN32
#include <conio.h>

/**
  コンソール消去用関数
  **/
#define SPC (0x20)
#define ATR (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)

#define GETCH _getch
#define KBHIT _kbhit

#else
#include <unistd.h>
#include <termios.h>
#include <curses.h>
//#include <kbhit.h>


//#define GETCH getch
//#define KBHIT kbhit


//#include "kbhit.h"
//#include <termios.h>
//#include <unistd.h>   // for read()

static struct termios initial_settings, new_settings;
static int peek_character = -1;

void init_keyboard()
{
    tcgetattr(0,&initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_lflag &= ~ISIG;
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
}

void close_keyboard()
{
    tcsetattr(0, TCSANOW, &initial_settings);
}

int KBHIT()
{
unsigned char ch;
int nread;

    if (peek_character != -1) return 1;
    new_settings.c_cc[VMIN]=0;
    tcsetattr(0, TCSANOW, &new_settings);
    nread = read(0,&ch,1);
    new_settings.c_cc[VMIN]=1;
    tcsetattr(0, TCSANOW, &new_settings);
    if(nread == 1)
    {
        peek_character = ch;
        return 1;
    }
    return 0;
}

int GETCH()
{
char ch;

    if(peek_character != -1)
    {
        ch = peek_character;
        peek_character = -1;
        return ch;
    }
    read(0,&ch,1);
    return ch;
}

#endif

void cls( void )
{
#ifdef WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD coord;
	HANDLE hOut;
	DWORD dwWrite;

	hOut = GetStdHandle( STD_OUTPUT_HANDLE );
	GetConsoleScreenBufferInfo( hOut, &csbi );
	dwWrite = (csbi.dwSize.X * csbi.dwSize.Y);
	coord.X = 0;
	coord.Y = 0;
	SetConsoleCursorPosition( hOut, coord );
	SetConsoleTextAttribute( hOut, ATR );
	FillConsoleOutputCharacter( hOut, SPC, dwWrite, coord, NULL );
	FillConsoleOutputAttribute( hOut, ATR, dwWrite, coord, NULL );
#endif
}

// Module specification
// <rtc-template block="module_spec">
static const char* dataporttest_spec[] =
  {
    "implementation_id", "DataPortTest",
    "type_name",         "DataPortTest",
    "description",       "DataPortTest",
    "version",           "1.0.0",
    "vendor",            "ysuga.net",
    "category",          "Test",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
DataPortTest::DataPortTest(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_longInIn("longIn", m_longIn),
    m_floatInIn("floatIn", m_floatIn),
    m_doubleInIn("doubleIn", m_doubleIn),
    m_longSeqInIn("longSeqIn", m_longSeqIn),
    m_floatSeqInIn("floatSeqIn", m_floatSeqIn),
    m_doubleSeqInIn("doubleSeqIn", m_doubleSeqIn),
    m_longOutOut("longOut", m_longOut),
    m_floatOutOut("floatOut", m_floatOut),
    m_doubleOutOut("doubleOut", m_doubleOut),
    m_longSeqOutOut("longSeqOut", m_longSeqOut),
    m_floatSeqOutOut("floatSeqOut", m_floatSeqOut),
    m_doubleSeqOutOut("doubleSeqOut", m_doubleSeqOut)

    // </rtc-template>
{
#ifndef WIN32
  init_keyboard();
#endif
}

/*!
 * @brief destructor
 */
DataPortTest::~DataPortTest()
{
#ifndef WIN32
  close_keyboard();
#endif
}



RTC::ReturnCode_t DataPortTest::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("longIn", m_longInIn);
  addInPort("floatIn", m_floatInIn);
  addInPort("doubleIn", m_doubleInIn);
  addInPort("longSeqIn", m_longSeqInIn);
  addInPort("floatSeqIn", m_floatSeqInIn);
  addInPort("doubleSeqIn", m_doubleSeqInIn);
  
  // Set OutPort buffer
  addOutPort("longOut", m_longOutOut);
  addOutPort("floatOut", m_floatOutOut);
  addOutPort("doubleOut", m_doubleOutOut);
  addOutPort("longSeqOut", m_longSeqOutOut);
  addOutPort("floatSeqOut", m_floatSeqOutOut);
  addOutPort("doubleSeqOut", m_doubleSeqOutOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t DataPortTest::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DataPortTest::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DataPortTest::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DataPortTest::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DataPortTest::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t DataPortTest::onExecute(RTC::UniqueId ec_id)
{
	cls();
	std::cout << "Data Port Test" << std::endl;
	std::cout << "Input >> " << std::endl;
	std::cout << "l:longOut\nf:floatOut\nd:doubleOut\n";
	std::cout << "L:longSeqOut\nF:floatSeqOut\nD:doubleSeqOut\n";

	char buffer[256];
	if(KBHIT()) {
		int c = GETCH();
#ifndef WIN32
		close_keyboard();
#endif
		rewind(stdin);
		switch(c) {
			case 'l':
				std::cout << "Input Number (long): ";
				std::cin  >> buffer;
				m_longOut.data = atoi(buffer);
				m_longOutOut.write();
				break;
			case 'f':
				std::cout << "Input Number (float): ";
				std::cin  >> buffer;
				m_floatOut.data = atof(buffer);
				m_floatOutOut.write();
				break;
			case 'd':
				std::cout << "Input Number (double): ";
				std::cin  >> buffer;
				m_doubleOut.data = atof(buffer);
				m_doubleOutOut.write();
				break;

			case 'L':
				std::cout << "Input Length (long): ";
				std::cin  >> buffer;
				m_longSeqOut.data.length(atoi(buffer));
				for(unsigned int i = 0;i < m_longSeqOut.data.length();i++) {
					std::cin >> buffer;
					m_longSeqOut.data[i] = atoi(buffer);
				}
				m_longSeqOutOut.write();
				break;
			case 'F':
				std::cout << "Input Length (float): ";
				std::cin  >> buffer;
				m_floatSeqOut.data.length(atoi(buffer));
				for(unsigned int i = 0;i < m_floatSeqOut.data.length();i++) {
					std::cin >> buffer;
					m_floatSeqOut.data[i] = atof(buffer);
				}
				m_floatSeqOutOut.write();
				break;
			case 'D':
				std::cout << "Input Length (double): ";
				std::cin  >> buffer;
				m_doubleSeqOut.data.length(atoi(buffer));
				for(unsigned int i = 0;i < m_doubleSeqOut.data.length();i++) {
					std::cin >> buffer;
					m_doubleSeqOut.data[i] = atof(buffer);
				}
				m_doubleSeqOutOut.write();
				break;
			default:
				break;
		}
#ifndef WIN32
		init_keyboard();
#endif
	}

	if(m_floatInIn.isNew()) {
		m_floatInIn.read();
	}
	if(m_longInIn.isNew()) {
		m_longInIn.read();
	}
	if(m_doubleInIn.isNew()) {
		m_doubleInIn.read();
	}
	if(m_floatSeqInIn.isNew()) {
		m_floatSeqInIn.read();
	}
	if(m_longSeqInIn.isNew()) {
		m_longSeqInIn.read();
	}
	if(m_doubleSeqInIn.isNew()) {
		m_doubleSeqInIn.read();
	}
	std::cout << "Long  :" << m_longIn.data << std::endl;
	std::cout << "Float :" << m_floatIn.data << std::endl;
	std::cout << "Double:" << m_doubleIn.data << std::endl;

	std::cout << "LongSeq:" << std::endl;
	for(unsigned int i = 0;i < m_longSeqIn.data.length();i++) {
		std::cout << m_longSeqIn.data[i] << ", ";
	}
	std::cout << std::endl;
	std::cout << "FloatSeq:" << std::endl;
	for(unsigned int i = 0;i < m_floatSeqIn.data.length();i++) {
		std::cout << m_floatSeqIn.data[i] << ", ";
	}
	std::cout << std::endl;
	std::cout << "Double:" << std::endl;
	for(unsigned int i = 0;i < m_doubleSeqIn.data.length();i++) {
		std::cout << m_doubleSeqIn.data[i] << ", ";
	}
	std::cout << std::endl;


  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t DataPortTest::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DataPortTest::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DataPortTest::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DataPortTest::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DataPortTest::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void DataPortTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(dataporttest_spec);
    manager->registerFactory(profile,
                             RTC::Create<DataPortTest>,
                             RTC::Delete<DataPortTest>);
  }
  
};


