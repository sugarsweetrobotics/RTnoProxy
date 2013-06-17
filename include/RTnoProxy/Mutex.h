#ifndef MUTEX_HEADER_INCLUDED
#define MUTEX_HEADER_INCLUDED

#include "libysuga.h"

#ifdef WIN32
#include <windows.h>
#else

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
	}
}



#endif //#ifndef MUTEX_HEADER_INCLUDED
