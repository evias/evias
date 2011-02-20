#include "regexp_counter.hpp"

namespace evias {

namespace core {

namespace regexp {

    occurenceCounter::occurenceCounter()
        : _minCount(1), _maxCount(1)
    {

    }

    occurenceCounter::~occurenceCounter()
    {

    }

    bool occurenceCounter::mayNotOccur()
    {
        return (_minCount == 0);
    }

    int occurenceCounter::getMin() const
    {
        return _minCount;
    }

    int occurenceCounter::getMax() const
    {
        return _maxCount;
    }

    void occurenceCounter::setMin(int min)
    {
        _minCount = min < 0 ? 0 : min;
    }

    void occurenceCounter::setMax(int max)
    {
        _maxCount = max < _minCount ? _minCount : max;
    }

}; // regexp

}; // core

}; // evias

