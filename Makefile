CXX=g++
CXXOPTIMIZE= -O2
CXXFLAGS= -g -Wall -pthread -std=c++0x $(CXXOPTIMIZE)
GTEST_DIR=googletest/googletest
SERVERCLASSES=config_parser.cc

default:  config_parser config_parser_test webserver reply.o request_handle.o

webserver: webserver.cc config_parser.h config_parser.cc request_handle.h reply.o request_handle.o
	g++ webserver.cc config_parser.cc reply.o request_handle.o -I /usr/local/Cellar/boost/1.54.0/include -std=c++11 -lboost_system -o webserver

reply.o: reply.cc reply.h
	g++ -c -std=c++11 reply.cc

request_handle.o: request_handle.cc request_handle.h reply.h
	g++ -c -std=c++11 request_handle.cc  

config_parser: config_parser.cc config_parser_main.cc
	$(CXX) -o $@ $^ $(CXXFLAGS)

config_parser_test:
	g++ -std=c++0x -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++0x -isystem ${GTEST_DIR}/include -pthread config_parser_test.cc config_parser.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o config_parser_test

clean:
	rm -rf *.o *~ *.gch *.swp *.dSYM config_parser config_parser_test *.tar.gz webserver reply.o request_handle.o




