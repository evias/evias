#ifndef EVIAS_CORE_NS_SINGLETON
#define EVIAS_CORE_NS_SINGLETON

#include "defines.h"
#include "notclonable.hpp"

#include <memory>

#ifdef __GNUC__
#  define SINGLETON_NAME __PRETTY_FUNCTION__
#elif defined(__STDC_VERSION__) && __STDC_VERSION__  >= 19901L
#  define SINGLETON_NAME __func__
#else
#  define SINGLETON_NAME __FUNCTION__
#endif

namespace evias {

namespace core {

    template <class __singletonClass>
    class Singleton : public NotClonable
    {
        public:
            virtual ~Singleton()
            {
                m_pSingleton = nullptr;
            }

            static __singletonClass& getSingleton()
            {
                if(m_pSingleton == nullptr)
                    throw std::exception(__FUNCTION__);

                return *m_pSingleton;
            }

            static __singletonClass* getSingletonPtr()
            {
                return m_pSingleton;
            }

        protected:
            Singleton(__singletonClass* in_p)
            {
                if(m_pSingleton)
                    throw std::exception("Already exists : " + __FUNCTION__);

                m_pSingleton = in_p;
            }

            Singleton() // to use only for Singleton derived class
            {
                if(m_pSingleton)
                    throw std::exception("Already exists : " + __FUNCTION__);

                m_pSingleton = (__singletonClass*) (this);
            }

        private:

            static __singletonClass* m_pSingleton;
    };

    template <class __singletonClass>
    __singletonClass* Singleton<__singletonClass>::m_pSingleton = nullptr;

/**
 * LazySingleton : handles auto construct- and destruct-ion.
 *
 * Class for users who don't want to "new" and "delete" their
 * singleton objects
 **/
    template <class __C>
    class LazySingleton : public NotClonable
    {
        public:
            static __C& getSingleton()
            {
                if(m_pSingleton.get() == nullptr)
                    m_pSingleton.reset(new __C());

                return *m_pSingleton;
            }

            static __C* getSingletonPtr()
            {
                if(! m_pSingleton)
                    m_pSingleton.reset(new __C());

                return m_pSingleton.get();
            }

            typedef std::unique_ptr<__C> Ptr;

        protected:
            LazySingleton() {};

            virtual ~LazySingleton() {};

        private:

            static std::unique_ptr<__C> m_pSingleton;

            friend class std::unique_ptr<__C>;
    };

    template <class __C>
    std::unique_ptr<__C> LazySingleton<__C>::m_pSingleton;

}; // core

}; // evias

#endif

