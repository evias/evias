#ifndef __EVIAS_CORE_ASSERTABLE__
#define __EVIAS_CORE_ASSERTABLE__

#include <exception>
#include <string>

namespace evias {

namespace core {

    using std::string;

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

    template <typename __strType>
    class assertableString
    {
    public :
        
        static void assertEqual(__strType, __strType);
        static void assertNotEqual(__strType, __strType);

        static void assertBiggerThan(__strType, __strType);
        static void assertSmallerThan(__strType, __strType);
    };

    // API IMPLEMENTATION

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

    // class assertableString

    template <typename __strType>
    void assertableString<__strType>::assertEqual(__strType v1, __strType v2)
    {
        if (strcmp(v1, v2) == 0) return ;

        throw (wrongAssert());
    }

    template <typename __strType>
    void assertableString<__strType>::assertNotEqual(__strType v1, __strType v2)
    {
        if (strcmp(v1, v2) != 0) return ;

        throw (wrongAssert());
    }

    template <typename __strType>
    void assertableString<__strType>::assertBiggerThan(__strType v1, __strType v2)
    {
        if (strcmp(v1, v2) == 1) return ;

        throw (wrongAssert());
    }

    template <typename __strType>
    void assertableString<__strType>::assertSmallerThan(__strType v1, __strType v2)
    {
        if (strcmp(v1, v2) == -1) return ;

        throw (wrongAssert());
    }
    
}; // end namespace core

}; // end namespace evias

#endif
