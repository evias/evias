#ifndef __EVIAS_CORE_FUNCTIONAL__
#define __EVIAS_CORE_FUNCTIONAL__

#include <exception>
#include <string>

namespace evias {

namespace core {

    /**
     * @brief
     * evias::core::functor class defines a workaround
     * for executing callback methods returning __returnType
     * and taking 2 parameters of type __valueType.
     *
     * This class allows an easy re-use of often used
     * callback methods.
     *
     * @todo implement class member function pointers
     *
     * @example
     * bool intEquals(int i1, int i2)
     * {
     *     return i1 == i2;
     * }
     *
     * typedef bool (*my_callback_t) (int, int);
     * evias::core::functor<bool, my_callback_t, int>* f = new evias::core::functor<bool, my_callback_t, int>();
     *
     * my_callback_t cb = NULL;
     * cb = &intEquals;
     * f->setCallback(cb);
     *
     * f->execute(1, 1); // return (bool) true
     * f->execute(1, 2); // return (bool) false
     * f->execute(42, 42); // return (bool) true
     * f->execute(42, 24); // return (bool) false, sadly
     **/
    template<typename __returnType, typename __callback, typename __valueType>
    class functor
    {
        __callback m_callback;

    public :
        functor(__callback callback=NULL);
        functor(const functor&);

        void setCallback(__callback);

        virtual ~functor() {};

        __returnType execute(__valueType, __valueType);
    };

    template<typename __returnType, typename __callback, typename __valueType>
    functor<__returnType, __callback, __valueType>::functor(__callback callback)
    {
        // will point to the callback function address
        m_callback = callback;
    }

    template<typename __returnType, typename __callback, typename __valueType>
    functor<__returnType, __callback, __valueType>::functor(const functor& r)
    {
    }

    template<typename __returnType, typename __callback, typename __valueType>
    void functor<__returnType, __callback, __valueType>::setCallback(__callback cb)
    {
        // point directly to the executed function
        m_callback = cb;
    }

    template<typename __returnType, typename __callback, typename __valueType>
    __returnType functor<__returnType, __callback, __valueType>::execute(__valueType v1, __valueType v2)
    {
        return (*m_callback)(v1, v2);
    }

}; // end namespace core

}; // end namespace evias

#endif
