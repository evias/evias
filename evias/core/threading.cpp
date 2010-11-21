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
        m_pImpl = new HANDLE;
        *((HANDLE*)m_pImpl) = CreateMutex(NULL, FALSE, NULL);
#else
        m_pImpl = new pthread_mutex_t;
        pthread_mutex_init((pthread_mutex_t*)m_pImpl, NULL);
#endif
    }

    Mutex::~Mutex()
    {
#if OS_WINDOWS
        HANDLE* pImpl = reinterpret_cast<HANDLE*>(m_pImpl);
        CloseHandle(*pImpl);
#else
        pthread_mutex_t* pImpl = reinterpret_cast<pthread_mutex_t*>(m_pImpl);
        pthread_mutex_destroy(pImpl);
#endif
        delete pImpl; (pImpl) = NULL;
        m_pImpl = NULL;
    }

    int Mutex::lock()
    {
        int iRet = RET_SUCCESS;

#if OS_WINDOWS
        if(WAIT_OBJECT_0 != WaitForSingleObject(*((HANDLE*)m_pImpl), INFINITE))
            iRet = -1;
#else
        if(0 != pthread_mutex_lock((pthread_mutex_t*)m_pImpl))
            iRet = -1;
#endif

        return iRet;
    }

    int Mutex::unlock()
    {
        int iRet = RET_SUCCESS;

#if OS_WINDOWS
        if(0 == ReleaseMutex(*((HANDLE*)m_pImpl)))
            iRet = -1;
#else
        if(0 != pthread_mutex_unlock((pthread_mutex_t*)m_pImpl))
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

