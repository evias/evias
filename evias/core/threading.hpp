#ifndef EVIAS_CORE_NS_THREADS
#define EVIAS_CORE_NS_THREADS

#include "defines.h"

#if OS_WINDOWS
#  define THREAD_FUNC void
#  define THREAD_RET
#  define __threadStart( function, callArgs ) { beginthread( (function), 0, (callArgs) ); }
#else
#  include <pthread.h>
#  define THREAD_FUNC void *
#  define THREAD_RET NULL
#  define __threadStart( function, callArgs ) { pthread_t threadObj; pthread_create( &threadObj, NULL, (function), (callArgs) ); }
#endif

namespace evias {

namespace core {

    class Lock;

    class Mutex
    {
        friend class Lock;

        protected:
            void *_handle;

            int lock();
            int unlock();

        public:
            Mutex();
            virtual ~Mutex();
    };

    class Lock
    {
        protected:
            Mutex& m;
        public:
            Lock(Mutex& m);
            ~Lock();
    };

}; // core

}; // evias

#endif

