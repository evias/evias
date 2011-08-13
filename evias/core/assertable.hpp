#ifndef __EVIAS_CORE_ASSERTABLE__
#define __EVIAS_CORE_ASSERTABLE__

#include <exception>

namespace evias {

namespace core {

    class wrongAssert : public std::exception
    {
    public :
        virtual const char* what() const
            throw() // this method will never throw
        {
            return "The assertion is wrong.";
        }
    };

    template <typename __valueType>
    class assertable
    {
    public :
        
        static void assertEqual(__valueType, __valueType);
        static void assertNotEqual(__valueType, __valueType);

        static void assertBiggerThan(__valueType, __valueType);
        static void assertSmallerThan(__valueType, __valueType);
    };

    // STATIC API IMPLEMENTATION

    template <typename __valueType>
    void assertable<__valueType>::assertEqual(__valueType v1, __valueType v2)
    {
        if (v1 == v2) return ;

        throw (wrongAssert());
    }

    template <typename __valueType>
    void assertable<__valueType>::assertNotEqual(__valueType v1, __valueType v2)
    {
        if (v1 != v2) return ;

        throw (wrongAssert());
    }

    template <typename __valueType>
    void assertable<__valueType>::assertBiggerThan(__valueType v1, __valueType v2)
    {
        if (v1 > v2) return ;

        throw (wrongAssert());
    }

    template <typename __valueType>
    void assertable<__valueType>::assertSmallerThan(__valueType v1, __valueType v2)
    {
        if (v1 < v2) return ;

        throw (wrongAssert());
    }

}; // end namespace core

}; // end namespace evias

#endif
