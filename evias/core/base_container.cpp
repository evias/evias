#include "base_container.hpp"

using namespace evias::core;

evias::core::baseContainer::baseContainer()
    : _dataPointer(0),
      _dataSize(0)
{

}

evias::core::baseContainer::baseContainer(const baseContainer& rhs)
{
    _dataSize    = rhs.getSize();

    // allocate enough memory for copying the content of the
    // right hand set.
    // Then copy data
    _dataPointer = reinterpret_cast<uint8_t*> (malloc(_dataSize+1));
    memcpy( _dataPointer, rhs.getData(), _dataSize );

    _dataPointer[_dataSize] = 0;
}

evias::core::baseContainer::baseContainer(uint64_t dataSize)
     : _dataPointer(0),
       _dataSize(dataSize)
{
    if (_dataSize) {
        // a size has been given, allocate memory and fill
        // with 0s
        _dataPointer = reinterpret_cast<uint8_t*> (malloc(_dataSize+1));
        memset( _dataPointer, 0, _dataSize+1 );
    }
}

evias::core::baseContainer::~baseContainer()
{
    // destruction logic

    releaseAlloc( _dataPointer );
    _dataSize = 0;
}

uint8_t* evias::core::baseContainer::getData()
{
    return _dataPointer;
}

const uint8_t* evias::core::baseContainer::getData() const
{
    return this->_dataPointer;
}

uint64_t evias::core::baseContainer::getSize() const
{
    return _dataSize;
}

void evias::core::baseContainer::extend(uint64_t additionalLen)
{
    this->resize(this->getSize() + additionalLen);
}

/**
 * \note having a smaller new size than the working one
 *       will pop some data of the container.
 **/
void evias::core::baseContainer::resize(uint64_t newLen)
{
    _dataPointer = reinterpret_cast<uint8_t*> (realloc(_dataPointer, newLen));
    if (newLen > _dataSize)
        // needs to fill added memory with 0s
        memset( _dataPointer + _dataSize, 0, newLen - _dataSize );

    _dataSize = newLen;
}

void evias::core::baseContainer::clear()
{
    releaseAlloc( _dataPointer );
    _dataSize = 0;
}

// return object must be deleted
Container* evias::core::baseContainer::getCopy() const
{
    baseContainer* copyObj = new baseContainer(this->getSize());
    memcpy(copyObj->getData(), this->getData(), this->getSize());

    return copyObj;
}

void evias::core::baseContainer::chain(const readOnlyContainer& toAppend)
{
    uint64_t oldSize = this->getSize();

    this->resize(oldSize + toAppend.getSize());
    memcpy( _dataPointer + oldSize, toAppend.getData(), toAppend.getSize() );
}

// XXX
uint32_t evias::core::baseContainer::md5Checksum() const
{
    return 0;
}

