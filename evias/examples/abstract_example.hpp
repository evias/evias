#ifndef __EVIAS_EXAMPLES_ABSTRACT_EXAMPLE__
#define __EVIAS_EXAMPLES_ABSTRACT_EXAMPLE__

#include <map>
#include <string>
#include <vector>

#include "../application/project.hpp"

namespace evias {

namespace examples {

    using evias::application::Project;
    using evias::application::consoleParser;

    using std::string;
    using std::map;
    using std::vector;

    class example : public Project
    {
        map<string,string>  m_args;
        vector<string>      m_rules;

    public :
        example(string);

        ~example();

        void setOptionsRules(vector<string>);

        int bootstrap(int, char**);
        virtual int execute()  = 0;
        virtual int shutdown() = 0;
    };

    // IMPLEMENTATION
    // class example

    example::example(string n)
        : Project(n)
    {
        setName(n);
    }

    example::~example()
    {
    }

    int example::bootstrap(int argc, char** argv)
    {
        setCallParser(new consoleParser(_name, "", argc, argv));

        if (! m_rules.empty()) {
            // parse call options
            for (vector<string>::iterator rule = m_rules.begin();
                 rule != m_rules.end(); rule++) {

                getCallParser()->addAllowedArg((*rule));
            }

            getCallParser()->parseAll();
            m_args = getCallParser()->readData();
        }

        return 0;
    }

    void example::setOptionsRules(vector<string> r)
    {
        // XXX format: "option_long,o=(empty|string|integer|boolean):description of the option"
        m_rules = r;
    }

}; // examples

}; // evias

#endif
