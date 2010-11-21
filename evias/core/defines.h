#ifndef EVIAS_CORE_DEFINES
#   define EVIAS_CORE_DEFINES

/** OS SPECIFIC **/
#   if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(_WIN32) || defined(_WIN32_) || defined(__CYGWIN__)
#       define OS_WINDOWS 1
#   else
#       define OS_WINDOWS 0
#   endif

/** UTILITIES **/
#   include <memory.h>
#   define call_member_func( object, ptrToMember ) ((object).*(ptrToMember))
#   define releaseMemory( object ) { if (object) { delete object; (object) = NULL} }
#   define releaseAlloc( object ) { if (object) { free(object); (object) = NULL; } }

#   define RET_SUCCESS  (int) 0

/**
    WORK AROUND FOR NULL POINTER
    USED FOR OLDER COMPILERS
 **/

const class {
    public:

        template<class C>
        operator C*() const { return 0; }

        template<class C, class D>
        operator D C::*() const { return 0; }
    private:

        void operator&() const;
} nullptr = {};


#endif /* EVIAS_CORE_DEFINES */

