#include "threading.hpp"

#if OS_WINDOWS
#  include <windows.h>
#else
#  include <pthread.h>
#endif

namespace evias {

namespace core {

    Mutex::Mutex()
    {
#if OS_WINDOWS
        _handle = new HANDLE;

        // CreateMutex returns an object reference to the created
        // handle (aka the mutex)
        *((HANDLE*)_handle) = CreateMutex(NULL, false, NULL);
#else
        _handle = new pthread_mutex_t;
        pthread_mutex_init((pthread_mutex_t*)_handle, NULL);
#endif
    }

    Mutex::~Mutex()
    {
        // destroy the mutex and free memory

#if OS_WINDOWS
        // from void* to HANDLE*
        HANDLE* handle = reinterpret_cast<HANDLE*>(_handle);
        CloseHandle(*_handle);
#else
        // from void* to pthread_mutex_t*
        pthread_mutex_t* handle = reinterpret_cast<pthread_mutex_t*>(_handle);
        pthread_mutex_destroy((pthread_mutex_t*) _handle);
#endif
        delete handle; (handle) = NULL;
        _handle = NULL;
    }

    int Mutex::lock()
    {
        int iRet = RET_SUCCESS;

#if OS_WINDOWS
        if(WAIT_OBJECT_0 != WaitForSingleObject(*((HANDLE*)_handle), INFINITE))
            iRet = -1;
#else
        if(0 != pthread_mutex_lock((pthread_mutex_t*)_handle))
            iRet = -1;
#endif

        return iRet;
    }

    int Mutex::unlock()
    {
        int iRet = RET_SUCCESS;

#if OS_WINDOWS
        if(0 == ReleaseMutex(*((HANDLE*)_handle)))
            iRet = -1;
#else
        if(0 != pthread_mutex_unlock((pthread_mutex_t*)_handle))
            iRet = -1;
#endif

        return iRet;
    }

/** class Lock **/

    Lock::Lock(Mutex& in_m)
        : m(in_m)
    {
        m.lock();
    }

    Lock::~Lock()
    {
        m.unlock();
    }

}; // core

}; // evias

