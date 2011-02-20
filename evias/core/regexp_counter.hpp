#ifndef EVIAS_CORE_REGEXP_NS_COUNTER
#define EVIAS_CORE_REGEXP_NS_COUNTER

namespace evias {

namespace core {

namespace regexp {

    class occurenceCounter
    {
    public :

        occurenceCounter();
        virtual ~occurenceCounter();

        bool mayNotOccur();

        int getMin() const;
        int getMax() const;

        void setMin(int);
        void setMax(int);

    private :

        int     _minCount;
        int     _maxCount;
    };

}; // regexp

}; // core

}; // evias

#endif

