# ----
# evias C++ library Makefile
#
# -- Compile instructions
#
# make library : compile evias C++ library
# make tests   : compile evias C++ library's unitary tests
# make all     : compile both, library and tests
# ----

LIB_VERSION=`cat VERSION`
CAT_CLONE_PATH=`cat /tmp/evias_clone_path`

# PATHS
JUNK_DIR=build/junk
LIB_TEST_JUNK_DIR=evias/tests/junk
LIB_TEST_PREFIX=evias/tests/bin
LIB_TEST_SRC_DIR=evias/tests
LIB_EXAMPLES_DIR=evias/examples
LIB_CORE_DIR=evias/core
LIB_APP_DIR=evias/application
LIB_MODELS_DIR=evias/models
LIB_NETWORK_DIR=evias/network

LIB_NAME=evias
LIB_RELEASE=1
LIB_PREFIX=build
LIB_SRC_DIR=evias

# COMPILATION
BOOST_PATH=/home/greg/srv/home.work/external/boost_1_47_0

PSQL_INC=-I/usr/include/pqxx
QT_INC=-I/usr/include/qt4
BOOST_INC=-I ${BOOST_PATH}

QT_LINK=-lQtGui -lQtCore
PSQL_LINK=-lpq -lpqxx
MD5_LINK=-lssl
BOOST_REGEX_LINK=${BOOST_PATH}/libs/regex/build/gcc/libboost_regex-gcc-1_47.a

EVIAS_LINK=-Lbuild/ -levias

CXX=g++
CXX_OPTS=-Ievias -Iplatform ${QT_INC} ${PSQL_INC} ${BOOST_INC} -fpermissive

MOC=/usr/bin/moc-qt4
MOC_BUILD=${CXX} ${QT_INC} -I./ -c
BUILD=${CXX} ${CXX_OPTS} -c

LIB_LINKER= ${CXX} ${MD5_LINK} ${PSQL_LINK} ${BOOST_REGEX_LINK} -shared -Wl,-soname,lib${LIB_NAME}.so
TEST_LINKER = ${CXX} ${EVIAS_LINK} ${QT_LINK} ${PSQL_LINK}
EXAMPLE_LINKER = ${CXX} ${EVIAS_LINK} ${QT_LINK} ${PSQL_LINK}
LINKER_ADD=${BOOST_REGEX_LINK}

# LIBRARY FILES

LIB_CORE =  common_utils        \
            string_utils        \
            date                \
            config_ini          \
            jsonParser          \
            db_adapter          \
            db_field            \
            sql_queries         \
            unit_test_abstract  \
			unit_test_suite		\
            threading           \
            container           \
            base_container		\
			run_chrono          \
            regexp

LIB_APPLICATION =	console          \
					project          \
					irc              \
                    logger           \
					MainWnd          \
					debugger         \
					TailEditor       \
                    basicView

LIB_MODEL = model_abstract
LIB_NETWORK =   packet          \
                connection      \
                tcpipConnection \
                net_functions

LIB_MOCS =  TailEditor		\
            MainWnd			\
			debugger		\
            basicView

clean :
	rm -f ${JUNK_DIR}/*.o
	rm -f ${LIB_PREFIX}/lib${LIB_NAME}.so
	rm -f ${LIB_PREFIX}/${LIB_NAME}.so.${LIB_RELEASE}
	rm -f $(foreach mocfile, ${LIB_MOCS}, ${LIB_APP_DIR}/moc_${mocfile}.hpp.cpp)
	rm -f $(foreach mocfile, ${LIB_MOCS}, ${LIB_APP_DIR}/moc_${mocfile}.cpp)
	rm -f ${LIB_TEST_JUNK_DIR}/*.o
	rm -f ${LIB_TEST_PREFIX}/*.exe
	rm -f ${LIB_TEST_PREFIX}/*${LIB_NAME}*
	rm -f ${LIB_EXAMPLES_DIR}/junk/*.o
	rm -f ${LIB_EXAMPLES_DIR}/bin/*${LIB_NAME}*
	rm -f ${LIB_EXAMPLES_DIR}/bin/*.exe

all :
	@make library
	@make tests
	@make examples

library :
	@echo " "
	@echo " "
	@echo "---- Start build of evias C++ library v${LIB_VERSION} ----"
	@make clean
	@echo " "
	@echo " "
	@echo "-- evias::core"
	$(foreach source, ${LIB_CORE}, `${BUILD} ${LIB_CORE_DIR}/${source}.cpp -o ${JUNK_DIR}/${source}.o`)
	@echo " "
	@echo " "
	@echo "-- evias::application"
	$(foreach source, ${LIB_APPLICATION}, `${BUILD} ${LIB_APP_DIR}/${source}.cpp -o ${JUNK_DIR}/${source}.o`)
	@echo " "
	@echo " "
	@echo "-- evias::application views "
	$(foreach source, ${LIB_MOCS}, `${MOC} ${LIB_APP_DIR}/${source}.hpp > ${LIB_APP_DIR}/moc_${source}.cpp`)
	$(foreach source, ${LIB_MOCS}, `${MOC_BUILD} ${LIB_APP_DIR}/moc_${source}.cpp -o ${JUNK_DIR}/moc_${source}.o`)
	@echo " "
	@echo " "
	@echo "-- evias::models"
	$(foreach source, ${LIB_MODEL}, `${BUILD} ${LIB_MODELS_DIR}/${source}.cpp -o ${JUNK_DIR}/${source}.o`)
	@echo " "
	@echo " "
	@echo "-- evias::network"
	$(foreach source, ${LIB_NETWORK}, `${BUILD} ${LIB_NETWORK_DIR}/${source}.cpp -o ${JUNK_DIR}/${source}.o`)
	@echo " "
	@echo " "
	@echo "-- link libevias.so"
	${LIB_LINKER} -o ${LIB_PREFIX}/${LIB_NAME}.so.${LIB_RELEASE} ${LINKER_ADD} $(foreach object, ${LIB_CORE} ${LIB_APPLICATION} ${LIB_MODEL} ${LIB_NETWORK}, ${JUNK_DIR}/${object}.o) $(foreach object, ${LIB_MOCS}, ${JUNK_DIR}/moc_${object}.o) -lc
	ln -s ${LIB_NAME}.so.${LIB_RELEASE} ${LIB_PREFIX}/lib${LIB_NAME}.so
	@echo " "
	@echo "-- libevias.so exported to ${LIB_PREFIX}"
	@echo " "
	@echo " "
	@echo "---- Build of evias C++ library v${LIB_VERSION} done ----"

tests :
	@echo " "
	@echo " "
	@echo "---- Start build of evias C++ library's v${LIB_VERSION} unitary tests ----"
	@echo " "
	@echo " "
	@echo " "
	@echo "-- build/link unitary test suite"
	@${BUILD} ${LIB_TEST_SRC_DIR}/library_test_suite.cpp -o ${LIB_TEST_JUNK_DIR}/library_test_suite.o
	@${BUILD} ${LIB_TEST_SRC_DIR}/suite_execution.cpp -o ${LIB_TEST_JUNK_DIR}/suite_execution.o
	@${TEST_LINKER} -o ${LIB_TEST_PREFIX}/suite_execution.exe ${LIB_TEST_JUNK_DIR}/suite_execution.o ${LIB_TEST_JUNK_DIR}/library_test_suite.o ${LINKER_ADD}
	@echo " "
	@echo " "
	@echo "-- copy library files to tests bin directory"
	@cp ${LIB_PREFIX}/*${LIB_NAME}* ${LIB_TEST_PREFIX}/
	@echo " "
	@echo " "
	@echo " "
	@echo "---- Build of evias C++ library's v${LIB_VERSION} unitary tests done ----"

examples :
	@echo " "
	@echo " "
	@echo "---- Start build of evias C++ library's v${LIB_VERSION} examples ----"
	@echo " "
	@echo " "
	@echo "-- build/link examples suite"
	@pwd > /tmp/evias_clone_path
	@echo "[pathsConfig]\nexamples_dir = ${CAT_CLONE_PATH}/evias/examples" > /tmp/evias_examples_config
	@${BUILD} ${LIB_EXAMPLES_DIR}/console_launcher.cpp -o ${LIB_EXAMPLES_DIR}/junk/console_launcher.o
	@${EXAMPLE_LINKER} -o ${LIB_EXAMPLES_DIR}/bin/console.exe ${LIB_EXAMPLES_DIR}/junk/console_launcher.o ${LINKER_ADD}
	@echo " "
	@echo " "
	@echo "-- copy library files to tests bin directory"
	@cp ${LIB_PREFIX}/*${LIB_NAME}* ${LIB_EXAMPLES_DIR}/bin/
