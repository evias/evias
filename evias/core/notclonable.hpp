#ifndef EVIAS_CORE_NS_NOTCLONABLE
#define EVIAS_CORE_NS_NOTCLONABLE

namespace evias {

namespace core {

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
