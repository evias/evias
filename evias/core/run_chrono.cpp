#include "run_chrono.hpp"

using namespace evias::core;

evias::core::runChrono::runChrono()
    : _dIntegral(0.0)
{
    this->reset();
}

/**
 * measure
 *
 * get time elapsed since last reset or construction.
 **/
double evias::core::runChrono::measure()
{
    _clock.doTick();
    _dIntegral += _clock.getElapsed();

    return _dIntegral;
}

/**
 * reset
 *
 * set back seconds counter.
 **/
double evias::core::runChrono::reset()
{
    double ret = this->measure();
    _dIntegral = 0.0;

    return ret;
}

/**
 *
 * class clockInfo
 *
 **/

evias::core::clockInfo::clockInfo()
    : _lastTick(0),
      _time(0)
{
#if ! OS_WINDOWS
    gettimeofday( &_tvStart, NULL );
#endif

    // we want to work with seconds
    _start = static_cast<double>(_getTicks()) / 1000.0;
}

evias::core::clockInfo::~clockInfo()
{
}

void evias::core::clockInfo::doTick()
{
    _lastTick = _time;
    _time     = static_cast<double>(_getTicks()) / 1000.0 - _start;

    while (! _secondTicks.empty() && _time - _secondTicks.front() > 1.0) {
        // remove last second's ticks
        _secondTicks.pop_front();
    }

    _secondTicks.push_back(_time);
}

double evias::core::clockInfo::getElapsed() const
{
    return _time - _lastTick;
}

double evias::core::clockInfo::getTime() const
{
    return _time;
}

double evias::core::clockInfo::ticksBySecond() const
{
    return _secondTicks.size();
}

/** access private **/
uint32_t evias::core::clockInfo::_getTicks()
{
#if OS_WINDOWS
    return GetTickCount();
#else
    struct timeval now;
    gettimeofday(&now, NULL);

    return (
        (now.tv_sec - _tvStart.tv_sec)*1000
        +
        (now.tv_usec - _tvStart.tv_usec)/1000
    );
#endif
}

