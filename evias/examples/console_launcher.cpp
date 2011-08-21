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

    void consoleSuite::delegate(int (consoleSuite::*_delegate_t) ())
    {
        (this->*_delegate_t)();
    }

    int consoleSuite::run()
    {
        delegate(&consoleSuite::intro);
    }

    int consoleSuite::intro()
    {
        cout    << "-------------------------------------------" << endl
                << "This is the eVias C++ library example suite" << endl
                << "Please choose one action in the menu below." << endl
                << endl
                << "1: Configuration files examples"    << endl
                << "2: Database objects examples"       << endl
                << "3: Language parsing examples"       << endl
                << "4: Funny tools"                     << endl
                << "0: Quit"                            << endl;

        switch (_waitForValidInput<int>("Your choice [0-4]: ")) {
            default:
            case 0:
                delegate(&consoleSuite::seeyousoon);
                break;

            case 1:
                delegate(&consoleSuite::menu_configFiles);
                break;

            case 2:
                // db objects
                delegate(&consoleSuite::notSupported);
                break;

            case 3:
                // lang parsing
                delegate(&consoleSuite::notSupported);
                break;

            case 4:
                // fun
                delegate(&consoleSuite::notSupported);
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
        cout    << "-------------------------------------------" << endl
                << "Please select an example to execute" << endl
                << endl
                << "1: Simple User/Group configuration file parse" << endl
                << "9: Back"                            << endl;

        switch (_waitForValidInput<int>("Your choice [1|9]: ")) {
            default:
            case 9:
                delegate(&consoleSuite::intro);
                break;

            case 1:
                delegate(&consoleSuite::example_config1);
                break;
        }
        
        delegate(&consoleSuite::toMenuOrNot);

        return 0;
    }

    int consoleSuite::toMenuOrNot()
    {
        switch (_waitForValidInput<int>("Go back to menu ? [1/0]: ")) {
            case 1:
                delegate(&consoleSuite::intro);
                break;

            default:
            case 0:
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
