#ifndef __EVIAS_EXAMPLES_SUITE__
#define __EVIAS_EXAMPLES_SUITE__

#include <sstream>
#include <string>
#include <vector>
#include <iostream>

namespace evias {

namespace examples {

    using std::stringstream;
    using std::string;
    using std::vector;
    using std::cout;
    using std::endl;
    using std::ends;
    using std::cin;

    class suite
    {
    public :
        suite() {};
        virtual ~suite() {};

        virtual int run() = 0;

        string renderQuestion(string, vector<unsigned char>);
        void renderLines(vector<string>);

    protected :

        template <typename _choice_t>
        _choice_t _waitForValidInput(string);
    };

    class consoleSuite : public suite
    {
    public:

        consoleSuite();
        ~consoleSuite();

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
            toMenuOrNot();
            return 0;
        }

    private:

    };

    // IMPLEMENTATION
    // class suite

    template <typename _choice_t>
    _choice_t suite::_waitForValidInput(string msg)
    {
        using namespace std;
        _choice_t choice;

        int i = 0;
        while (true) {
            try {
                // try and get choice again
                cin.clear();

                if (i++ > 0) {
                    cout << endl << msg;
                }
                
                if (! (cin >> choice)) {
                    throw 1;
                }

                // valid choice
                return choice;
            }
            catch(int) {
                cout << endl << "Invalid input" << endl;

                cin.clear();
                while (cin.get() != '\n');
            }
        }
    }

    string suite::renderQuestion(string question, vector<unsigned char> choices)
    {
        string abbr = "[";
        vector<unsigned char>::const_iterator it = choices.begin();
        
        for (int i = 0; it != choices.end(); it++, i++) {
            char *sz = new char(2);
            if (i > 0)
                abbr.append(string(","));

            sz[0] = *it;
            sz[1] = '\0';
            abbr.append((const char*) sz);

            delete sz;
        }
        abbr.append("]");

        stringstream ss;
        ss << question << " " << abbr << ": " << ends;

        cout << ss.str();

        return ss.str();
    }

    void suite::renderLines(vector<string> lines)
    {
        for (vector<string>::const_iterator it = lines.begin();
             it != lines.end(); it++) {

            cout << (*it) << endl;
        }
    }

};

};

#endif
