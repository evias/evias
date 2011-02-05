#ifndef __EVIAS_CORE_READONLY_CONTAINER__
#   define __EVIAS_CORE_READONLY_CONTAINER__

#include "container.hpp"

namespace evias {

namespace core {

    class readOnlyContainer : public Container
    {
    protected:

        const uint8_t* const    _dataPointer;
        const uint64_t          _dataSize;

    public:

        readOnlyContainer() : _dataPointer(0), _dataSize(0) {};
        readOnlyContainer(const Container&);
        readOnlyContainer(const uint8_t* const, uint64_t);
        readOnlyContainer(const void* const, uint64_t);

        virtual ~readOnlyContainer();

        virtual Container*      getCopy() const;

        virtual const uint8_t*  getData() const;
        virtual uint64_t        getSize() const;
    };

}; // core

}; // evias

#endif
