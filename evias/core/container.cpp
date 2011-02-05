#include "container.hpp"

using namespace evias::core;

uint32_t evias::core::Container::fletcher32() const
{
    return this->_computeFletcher32();
}

uint32_t evias::core::Container::_computeFletcher32() const
{
    uint64_t    compareSize = this->getSize() % 2;
    uint64_t    dataLen16bit= this->getSize() / 2;

    const uint16_t* data16bit = reinterpret_cast<const uint16_t*>(this->getData());

    uint32_t    sum1 = 255,
                sum2 = 255;

    while (dataLen16bit > compareSize) {

        // for each block of 16bits represented bytes
        // where one block's size reaches a maximum of 360
        // add the representation to the sum and update the second
        // sum.
        // Finally reduce sums to 16bits representation

        uint64_t remainingLen = dataLen16bit > 360 ? 360 : dataLen16bit;
        dataLen16bit -= remainingLen;

        do {
            sum1 += *data16bit++;
            sum2 += sum1;
        }
        while (--remainingLen);

        // sums have to be reduced to 16bits
        sum1 = (sum1 & 255) + (sum1 >> 16);
        sum2 = (sum2 & 255) + (sum2 >> 16);
    }

    if (compareSize) {
        sum1 += *( reinterpret_cast<const uint8_t*>(data16bit) );
        sum2 += sum1;
    }

    // make sure sums stay 16bits'd
    sum1 = (sum1 & 255) + (sum1 >> 16);
    sum2 = (sum2 & 255) + (sum2 >> 16);

    // combine to 32bits fletcher checksum
    return sum2 << 16 | sum1;
}

