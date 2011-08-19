# eVias C++ library

Some of the features may be buggy or their implementation may be incomplete.
This has started as a hobbie development, helping me for a faster development
of applications using C++. Bugs can be reported by mail to
<saive.gregory@gmail.com>.

Mostly, this library will include features helping me to achieve daily tasks. The main
purpose of having this library linked to my applications is to be able to develop
those application much faster by having a good library layer to work on.

## Requirements

* g++/gcc -v >= 4.3
* libboost v1.47 is the minimum
* libpqxx
* libssl-dev
* libqt4-dev

The boost package is not yet available as an apt package so you have to compile
the extracted package yourself. (see bootstrap.sh)

## Use instructions

Compiling and linking the library can be done using

    make library

Compiling the unitary test suite (after library compile)

    make tests

Compiling the examples is done using :

    make examples

Though before being able to use the linked library, you have
to specify that LD_LIBRARY_PATH should look into the built
directory .. (evias/build)

Executing the test suite on your platform allows you to see if you
can use all the features proposed.

To execute the library unitary test suite, you execute the following
command :

    ./evias/tests/bin/suite_execution.exe

To compile another project using this library in any case, you have
to define the include path to the evias library header files and
link your executable with the evias library. As the examples
below will explain.

Take this as your base compilation rule
    g++ -c main.cpp -o main.o

So add the include path to the evias library
    g++ -Ipath/to/evias -c main.cpp -o main.o

Take this as you're base build rule
    g++ -o build/outputExecutable main.o

And add the needed information for linking to the evias library
    g++ -Lpath/to/evias -levias -o build/outputExecutable main.o

## Contribute

I would appreciate any contributions. This project is open source and
has no limit of extension. Any kind of module or concept may be implemented
in the library. I would be pleased to merge your work.

Again to contribute, you can use github.com's simple fork requests or you
can send me an email with a patch of your commit(s).

## Modules

- evias::core
    - configIni     : ".ini" file configuration read / write module
    - jsonSingleEntry  : JSON "key":"value"
    - jsonArrayEntry   : JSON "key":[array_entries]
    - jsonObjectEntry  : JSON "key":{object_entries}
    - dbAdapter     : database adapter workaround
    - pSqlAdapter   : PostgreSQL database adapter
    - dbTable          : database table data fetch
    - dbField          : database fields representation
    - dbRow            : simple row representation
    - sqlSelect        : SQL SELECT statement representation
    - sqlFrom          : SQL FROM statement representation
    - sqlWhere         : SQL WHERE statement representation
    - selectQuery      : represents a whole select statement (SQL)
    - updateQuery      : represents a whole update statement (SQL)
    - removeQuery      : represents a whole delete statement (SQL)
    - insertQuery      : represents a whole insert statement (SQL)
    - regex         : class working with boost::xpressive. Regular expressions module.
    - callback      : class for working with function pointers
    - callqueue     : class for managing a queue of callback call configurations
    - assertable    : class offering templated static functions for assertions.

- evias::application
    - consoleParser : console call arguments parsing module
    - Irc           : Internet Relay Chat client instance class
    - Project       : eVias Project abstraction layer
    - MainWnd       : Qt Main window
    - Debugger      : application debugger can be used for multiple projects
    - basicView     : inherits QWidget

- evias::network
    - netConnection     : abstracted class using 32bits packets socket based connections.
    - tcpipConnection   : TCP/IP standardized connection
    - netPacket         : class name, be enough..

- evias::core::test
    - unitTestSuite     : unitary test suite object, can run multiple unitary tests and print their results.
    - unitTest          : abstracted unitary test class base
      - configFiles     : tests the different configuration files reading features
      - jsonObjects     : tests the JSON parse / write objects.
      - sqlObjects      : tests the various SQL classes.
      - qtViews         : tests the Qt derivated views.
      - databaseObjects : tests the database connection objects. (PgSQL for now)
      - networkObjects  : tests the network objects (netPacket, tcpipConnection ..)
      - regularExpressions: tests the regular expression parse feature.
      - functional      : tests the functional part of the core library.
