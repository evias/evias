#include "suite.hpp"

#include "console/config1.hpp"

#include <iostream>

namespace evias {

namespace examples {

    using namespace std;

    consoleSuite::consoleSuite()
    {

    }

    consoleSuite::~consoleSuite()
    {

    }

    int consoleSuite::run()
    {
        intro();
    }

    int consoleSuite::intro()
    {
        string l[9] = {
            "-------------------------------------------",
            "This is the eVias C++ library example suite",
            "Please choose one action in the menu below.",
            "",
            "1: Configuration files examples",
            "2: Database objects examples",
            "3: Language parsing examples",
            "4: Funny tools",
            "0: Quit"
        };
        unsigned char c[5] = {'0','1','2','3','4'};

        renderLines(vector<string>(l, l+9));

        string question = renderQuestion("Action ", vector<unsigned char>(c, c+5));
        switch (_waitForValidInput<int>(question)) {
            default:
            case 0:
                seeyousoon();
                break;

            case 1:
                menu_configFiles();
                break;

            case 2:
                // db objects
                notSupported();
                break;

            case 3:
                // lang parsing
                notSupported();
                break;

            case 4:
                // fun
                notSupported();
                break;
        }

        return 0;
    }

    int consoleSuite::seeyousoon()
    {
        cout << endl
             << "Thanks for executing me ! Hope to see you soon. :o]" << endl
             << "-------------------------------------------" << endl
             << endl;

        return 0;
    }

    int consoleSuite::menu_configFiles()
    {
        string l[5] = {
            "-------------------------------------------",
            "Please select an example to execute",
            "",
            "1: Simple User/Group configuration file parse",
            "0: Back"
        };
        unsigned char c[2] = {'1','0'};

        renderLines(vector<string>(l, l+5));

        string question = renderQuestion("Action", vector<unsigned char>(c,c+2));
        switch (_waitForValidInput<unsigned char>(question)) {
            case '1':
                example_config1();
                break;

            case '0':
                intro();
                return 0;
        }

        toMenuOrNot();

        return 0;
    }

    int consoleSuite::toMenuOrNot()
    {
        unsigned char c[2] = {'y', 'n'};

        string question = renderQuestion("Go back to menu ?", vector<unsigned char>(c,c+2));
        switch (_waitForValidInput<unsigned char>(question)) {
            case 'Y':
            case 'y':
                intro();
                break;

            default:
                break;
        }
    }

    int consoleSuite::example_config1()
    {
        using evias::examples::config1;

        config1* c = new config1();

        c->execute();
        c->shutdown();

        return 0;
    }

};

};

int main (int argc, char* args[])
{
    using evias::examples::consoleSuite;

    consoleSuite* s = new consoleSuite();

    return s->run();
}
