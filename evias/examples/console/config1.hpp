#ifndef __EVIAS_EXAMPLES_CONSOLE_CONFIG1__
#define __EVIAS_EXAMPLES_CONSOLE_CONFIG1__

#include <iostream>

#include "../abstract_example.hpp"
#include "../../core/string_utils.hpp"

namespace evias {

namespace examples {

    using evias::examples::example;
    using evias::core::configIni;
    using evias::core::iniSection;
    using evias::core::trim;

    using namespace std;

    class config1 : public example
    {
    public :

        config1();

        int execute();
        int shutdown();
    };

    // IMPLEMENTATION
    // class config1

    config1::config1()
        : example("Configuration file features use example")
    {
    }

    int config1::execute()
    {
        // @see Makefile, I have introduced this little work around to
        // avoid you the changing of the files locations.
        // file /tmp/evias_examples_config is created at compile time

        configIni* conf = new configIni("/tmp/evias_examples_config");
        conf->parse();

        string examplesPath = conf->getValue("pathsConfig", "examples_dir");

        // XXX fix trim
        int startPos = 0;
        while (examplesPath[startPos] == ' ')
            startPos++;

        examplesPath = examplesPath.substr(startPos);

        configIni* example1 = new configIni(examplesPath + "/bin/config/config1-ex1.ini");
        example1->parse();

        if (! example1->state())
            cout << endl << "Something went wrong reading '" << examplesPath << "/bin/config/config1-ex1.ini'"
                 << endl;

        map<string, vector<string> > adapter;

        // XXX configIni::iterator
        vector<iniSection> sections = example1->getIniSections();
        vector<iniSection>::iterator it = sections.begin();
        for (; it != sections.end(); it++) {
            map<string,string> pairs = (*it).getPairs();
            map<string,string>::iterator pa = pairs.begin();

            if ((*it).getLabel() == "Users") {
                for (; pa != pairs.end(); pa++) {
                    map<string,vector<string> >::iterator f = adapter.begin();
                    bool found = false;
                    for (; f != adapter.end(); f++)
                        if (f->first == pa->second) {
                            found = true;
                            break;
                        }

                    if (found) {
                        adapter[pa->second].push_back(pa->first);
                    }
                    else {
                        vector<string> u;
                        u.push_back(pa->first);
                        adapter.insert(pair<string,vector<string> >(pa->second, u));
                    }
                }
            }
        }

        cout << endl
             << "------------------------" << endl
             << "Example output buffering" << endl
             << endl;
        map<string, vector<string> >::iterator current = adapter.begin();
        for (; current != adapter.end(); current++) {

            cout << endl
                 << "-- Group: '" << current->first << "'" << endl;

            vector<string>::iterator user = current->second.begin();
            for(; user != current->second.end(); user++)
                cout << "- " << *user << endl;
        }

        cout << endl
             << "------------------------"
             << endl;
        
        return 0;
    }

    int config1::shutdown()
    {
        return 0;
    }

};

};

#endif
