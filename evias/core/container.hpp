#ifndef EVIAS_CORE_NS_CONTAINER
#   define EVIAS_CORE_NS_CONTAINER

#include "defines.h"
#include <cstdlib>

namespace evias {

namespace core {

    class Container
    {
    public:

        virtual const uint8_t*  getData() const = 0;
        virtual uint64_t        getSize() const = 0;

        virtual uint32_t        fletcher32() const;
        virtual uint32_t        md5Checksum() const = 0;

    protected:

        Container() {};
        Container(const Container&) {};

    private:

        virtual uint32_t _computeFletcher32() const;

    };

}; // core

}; // evias

#endif

