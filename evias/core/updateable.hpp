#ifndef EVIAS_CORE_NS_UPDATEABLE
#   define EVIAS_CORE_NS_UPDATEABLE

namespace evias {

namespace core {

    /**
     * Updateable
     *
     * interface for objects which may be updated during run time
     *
     * the update algorithm and the actual update of the widget or
     * similar have to be implemented in the update() function.
     *
     **/
    class Updateable
    {
        public :

            Updateable() {};

            virtual Updateable() {};

            virtual int update() = 0;
    };

}; // core

}; // evias

#endif /* EVIAS_CORE_NS_UPDATEABLE */
