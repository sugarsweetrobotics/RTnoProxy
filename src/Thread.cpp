#include "Thread.h"

using namespace net::ysuga;

Thread::Thread(void)
{

}

Thread::~Thread(void)
{

}


THREAD_ROUTINE StartRoutine(void* arg)
{
	Thread* threadObject = (Thread*)arg;
	threadObject->run();
	threadObject->exit(0);
#ifdef WIN32
	ExitThread(0);
#else 
	pthread_exit(0);
#endif
	return 0;
}


void Thread::start()
{	
#ifdef WIN32
	m_Handle = CreateThread(NULL, 0, StartRoutine, (LPVOID)this, 0, &m_ThreadId);
#else


#endif
}

void Thread::join()
{
#ifdef WIN32
	WaitForSingleObject(m_Handle, INFINITE);
#else

#endif
}

void Thread::sleep(unsigned long milliSeconds)
{
#ifdef WIN32
	::Sleep(milliSeconds);
#else

#endif
}

void Thread::exit(unsigned long exitCode) {
#ifdef WIN32
	ExitThread(exitCode);
#else
	pthread_exit(0);
#endif
}
