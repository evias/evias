#include "ro_container.cpp"

using namespace evias::core;

evias::core::readOnlyContainer::readOnlyContainer()
     : _dataPointer(0),
       _dataSize(0)
{
}

evias::core::readOnlyContainer::readOnlyContainer(const Container* copy)
     : _dataPointer(copy->getData()),
       _dataSize(copy->getSize())
{
}

evias::core::readOnlyContainer::readOnlyContainer(const uint8_t* pointer, uint64_t size)
     : _dataPointer(pointer),
       _dataSize(size)
{
}

evias::core::readOnlyContainer::readOnlyContainer(const void* pointer, uint64_t size)
     : _dataPointer(reinterpret_cast<const uint8_t*> (pointer)),
       _dataSize(size)
{

}

evias::core::readOnlyContainer::~readOnlyContainer()
{

}

Container* evias::core::readOnlyContainer::getCopy() const
{
    return new readOnlyContainer(this->getData(), this->getSize());
}

const uint8_t* evias::core::readOnlyContainer::getData() const
{
    return _dataPointer;
}

uint64_t evias::core::readOnlyContainer::getSize() const
{
    return _dataSize;
}

