#include "Mutex.h"

using namespace net::ysuga;

Mutex::Mutex()
{
#ifdef WIN32
	m_Handle = ::CreateMutex(NULL, 0, NULL);
#endif
}


Mutex::~Mutex()
{
#ifdef WIN32
	::CloseHandle(m_Handle);
#else

#endif
}

void Mutex::lock()
{
#ifdef WIN32
	::WaitForSingleObject(m_Handle, INFINITE);
#else

#endif
}


void Mutex::unlock()
{
#ifdef WIN32
	::ReleaseMutex(m_Handle);
#else

#endif
}