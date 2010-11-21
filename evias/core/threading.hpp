#ifndef EVIAS_CORE_NS_THREADS
#define EVIAS_CORE_NS_THREADS

#include "defines.h"

#if OS_WINDOWS
#  define THREAD_FUNC void
#  define THREAD_RET
#  define __threadStart( fct, arg ) { beginthread( (fct), 0, (arg) ); }
#else
#  include <pthread.h>
#  define THREAD_FUNC void *
#  define THREAD_RET NULL
#  define __threadStart( fct, arg ) { pthread_t _a_b_thread1_c_; pthread_create( &_a_b_thread1_c_, NULL, (fct), (arg) ); }
#endif

namespace evias {

namespace core {

    class Lock;

    class Mutex
    {
        friend class Lock;

        protected:
            void *m_pImpl;

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

