#ifndef EVIAS_CORE_NS_BASECONTAINER
#   define EVIAS_CORE_NS_BASECONTAINER

#include "container.hpp"
#include "ro_container.hpp"

namespace evias {

namespace core {

    class baseContainer : public Container
    {
    protected:

        uint8_t*    _dataPointer;
        uint64_t    _dataSize;

    public:

        baseContainer();
        baseContainer(const baseContainer&);
        baseContainer(uint64_t);

        virtual ~baseContainer();

        virtual Container*  getCopy() const;

        // get pointers to data (modifiable or not)
        virtual uint8_t*        getData();
        virtual const uint8_t*  getData() const;

        virtual uint64_t        getSize() const;

        virtual void resize(uint64_t);
        virtual void extend(uint64_t);
        virtual void clear();

        virtual void chain(const readOnlyContainer&);

        virtual uint32_t md5Checksum() const;
    };

}; // core

}; // evias

#endif

