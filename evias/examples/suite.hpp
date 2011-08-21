#ifndef __EVIAS_EXAMPLES_SUITE__
#define __EVIAS_EXAMPLES_SUITE__

#include <string>
#include <iostream>

namespace evias {

namespace examples {

    class suite
    {
    public :
        suite() {};
        virtual ~suite() {};

        virtual int run() = 0;

    protected :

        template <typename _choice_t>
        _choice_t _waitForValidInput(std::string);
    };

    class consoleSuite : public suite
    {
        typedef int (consoleSuite::*_delegate_t) ();

    public:

        consoleSuite();
        ~consoleSuite();
        
        void delegate(_delegate_t);

        int run();

    protected:

        int intro();
        int seeyousoon();
        int toMenuOrNot();
        int menu_configFiles();

        int example_config1();

        inline int notSupported()
        {
            using std::cout;using std::endl;
            cout << endl << "Feature not supported yet" << endl;
            delegate(&consoleSuite::toMenuOrNot);
            return 0;
        }

    private:

    };

    template <typename _choice_t>
    _choice_t suite::_waitForValidInput(std::string msg)
    {
        using namespace std;
        _choice_t choice;

        cout << endl
             << msg;
        cin >> choice;

        if (! cin) {
            while (true) {
                try {
                    cin.clear();
                    cout << endl << msg;
                    if (! (cin >> choice)) {
                        throw 1;
                    }
                    break;
                }
                catch(int) {
                    cout << endl << "Invalid input" << endl;

                    cin.clear();
                    while (cin.get() != '\n');
                }
            }
        }

        return choice;
    }

};

};

#endif
