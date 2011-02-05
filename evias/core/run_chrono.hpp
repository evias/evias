#ifndef EVIAS_CORE_NS_RUNCHRONO
#define EVIAS_CORE_NS_RUNCHRONO

#include "defines.h"

#include <list>
#include <functional>
#include <algorithm>

#if ! OS_WINDOWS
#   include <sys/time.h>
#endif

namespace evias {

namespace core {

    class clockInfo
    {
        public :
            clockInfo();
            virtual ~clockInfo();

            virtual void doTick();

            double getElapsed() const;
            double getTime() const;
            double ticksBySecond() const;

        private :

            uint32_t    _getTicks();
#if ! OS_WINDOWS
            struct timeval  _tvStart;
#endif

            double  _start;
            double  _lastTick;
            double  _time;

            std::list<double>   _secondTicks;
    };


    class runChrono
    {
        public:
            runChrono();

            virtual double measure();
            virtual double reset();

        private:
            clockInfo _clock;
            double _dIntegral;
    };

}; // core

}; // evias

#endif
