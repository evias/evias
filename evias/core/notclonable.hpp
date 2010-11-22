#ifndef EVIAS_CORE_NS_NOTCLONABLE
#define EVIAS_CORE_NS_NOTCLONABLE

namespace evias {

namespace core {

    /**
     * NotClonable
     *
     * interface for objects which may not be copied or cloned.
     *
     * The constructor may be implemented in sub classes.
     *
     * A brief but simple example of a use for this interface
     * is a singleton designed class implementation. In fact,
     * a singleton class will have one and only one instance
     * created at runtime which means there should not be any
     * possibility of redefining an instance for that class
     * or copying the content of the current instance to
     * a new one of the same class.
     **/
    class NotClonable
    {
        private:

            NotClonable(const NotClonable&);
            const NotClonable& operator=(const NotClonable&);

        protected:

            NotClonable() {};
            virtual ~NotClonable() {};
    };

};

};

#endif
