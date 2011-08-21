#ifndef __EVIAS_CORE_ASSERTABLE__
#define __EVIAS_CORE_ASSERTABLE__

#include <exception>
#include <string>
#include <sstream>

#include "functional.hpp"

namespace evias {

namespace core {

    using std::string;

    // the template on the exception class
    // allows having type informations displayed on runtime
    template <typename _a_assertion_t>
    class wrongAssert : public std::exception
    {
        string _info;

    public :
        wrongAssert(string info)
            : std::exception(), _info(info)
        {
        };

        virtual ~wrongAssert() throw() {};

        inline void setInfo(string info)
            { _info = info; }

        inline string getInfo() const
            { return _info.empty() ? "The assertion is wrong." : _info; }

        virtual const char* what() const
            throw() // this method will never throw
        {
            string processed = getInfo();

            string info = string(" Infos: '") + processed + string("'");
            return info.c_str();
        }
    };

    template <typename __valueType>
    class assertable
    {
        typedef bool (*__callback) (__valueType,__valueType);

    public :
        assertable() {};
        virtual ~assertable() {};

        static void assertEqual(__valueType, __valueType, string = "");
        static void assertEqual(__valueType, __valueType, __callback, string = "");
        static void assertNotEqual(__valueType, __valueType, string = "");

        static void assertBiggerThan(__valueType, __valueType, string = "");
        static void assertSmallerThan(__valueType, __valueType, string = "");
    };

    // API IMPLEMENTATION

    template <typename __valueType>
    void assertable<__valueType>::assertEqual(__valueType v1, __valueType v2, string info)
    {
        if (v1 == v2) return ;

        string msg = (info.empty() ? "assertEqual operator==(v1,v2) call" : info);
        throw (wrongAssert<__valueType>(msg.append(" is wrong")));
    }

    /**
     * @brief
     * this function allows comparing two specified values of one common
     * type which may require more than a simple operator==() call.
     * The pretty basical rules are to compare two values of type
     * __valueType (see template def) returning a boolean value.
     *
     * @param   v1              __valueType   The first variable for the comparison
     * @param   v2              __valueType   The second variable for the comparison
     * @param   callback_fptr   bool (*) (__valueType, __valueType)     The function pointer to be executed
     *
     * @return void
     **/
    template <typename __valueType>
    void assertable<__valueType>::assertEqual(__valueType v1, __valueType v2, bool (*callback_fptr) (__valueType,__valueType), string info)
    {
        typedef bool (*callback_t) (__valueType, __valueType);
        typedef evias::core::callback<bool, __valueType, callback_t> callback_c;

        callback_c* handler = new callback_c(callback_fptr);

        // execute callback method through functor
        if (handler->execute(v1, v2)) { delete handler; return ; }

        string msg = (info.empty() ? "assertEqual callback operator (*callback_ptr)(v1,v2) call" : info);
        throw (wrongAssert<__valueType>(msg.append(" is wrong")));
    }

    template <typename __valueType>
    void assertable<__valueType>::assertNotEqual(__valueType v1, __valueType v2, string info)
    {
        if (v1 != v2) return ;

        string msg = (info.empty() ? "assertEqual operator!=(v1,v2) call" : info);
        throw (wrongAssert<__valueType>(msg.append(" is wrong")));
    }

    template <typename __valueType>
    void assertable<__valueType>::assertBiggerThan(__valueType v1, __valueType v2, string info)
    {
        if (v1 > v2) return ;

        string msg = (info.empty() ? "assertEqual operator>(v1,v2) call" : info);
        throw (wrongAssert<__valueType>(msg.append(" is wrong")));
    }

    template <typename __valueType>
    void assertable<__valueType>::assertSmallerThan(__valueType v1, __valueType v2, string info)
    {
        if (v1 < v2) return ;

        string msg = (info.empty() ? "assertEqual operator<(v1,v2) call" : info);
        throw (wrongAssert<__valueType>(msg.append(" is wrong")));
    }
    
}; // end namespace core

}; // end namespace evias

#endif
