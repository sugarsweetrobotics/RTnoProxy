/********************************************************
 * SerialPort.h
 *
 * Portable Thread Class Library for Windows and Unix.
 * @author ysuga@ysuga.net
 * @date 2010/11/02
 ********************************************************/

#ifndef THREAD_HEADER_INCLUDED
#define THREAD_HEADER_INCLUDED

#include "libysuga.h"

#ifdef WIN32
#include <windows.h>
#define THREAD_ROUTINE DWORD WINAPI
#else
#include <pthread.h>
#define THREAD_ROUTINE void*
#endif


namespace net {
	namespace ysuga {

		/**
		 *
		 */
		class LIBYSUGA_API Mutex {
		private:
#ifdef WIN32
		         HANDLE m_Handle;
#else
			
#endif
		public:
			Mutex();
			virtual ~Mutex();

		public:
			void lock();

			void unlock();
		};


		/**
		 *
		 */
		class LIBYSUGA_API Thread
		{
		private:
#ifdef WIN32
			HANDLE m_Handle;
			DWORD m_ThreadId;
#else
			// pthread_t m_Handle;
			
#endif
		public:
			Thread(void);
			virtual ~Thread(void);

		public:
			void start();
			virtual void run() {};

			void join();

			void exit(unsigned long exitCode);

		public:
			static void sleep(unsigned long milliSeconds);
		};

	};
};


#endif

/*******************************************************
 * Copyright  2010, ysuga.net all rights reserved.
 *******************************************************/
