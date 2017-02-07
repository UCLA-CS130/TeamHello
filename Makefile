CXX=g++
CXXOPTIMIZE= -O2
CXXFLAGS= -g -Wall -pthread -std=c++11 $(CXXOPTIMIZE)
GTEST_DIR=googletest/googletest
SERVERCLASSES=config_parser.cc

default:  config_parser config_parser_test reply.o reply_echo.o reply_static.o mime_types.o request_handle.o webserver webserver_test request_handle_test reply_echo_test reply_static_test

reply.o: reply.cc reply.h mime_types.h mime_types.cc
	g++ -c -std=c++11 reply.cc -lboost_system

reply_static.o: reply_static.cc reply_static.h reply.h mime_types.h mime_types.cc
	g++ -c -std=c++11 reply_static.cc -lboost_system

reply_echo.o: reply_echo.cc reply_echo.h reply.h mime_types.h mime_types.cc
	g++ -c -std=c++11 reply_echo.cc -lboost_system

request_handle.o: request_handle.cc request_handle.h reply.h reply_echo.h reply_static.h
	g++ -c -std=c++11 request_handle.cc  -lboost_system

mime_types.o: mime_types.cc mime_types.h
	g++ -c -std=c++11 mime_types.cc

webserver: webserver.h webserver.cc webserver_main.cc config_parser.h config_parser.cc request_handle.h reply.o request_handle.o mime_types.o reply_static.o reply_echo.o
	g++ webserver.h webserver.cc webserver_main.cc config_parser.cc reply.o request_handle.o mime_types.o reply_static.o reply_echo.o -I /usr/local/Cellar/boost/1.54.0/include -std=c++11 -lboost_system -o webserver


config_parser: config_parser.cc config_parser_main.cc
	$(CXX) -o $@ $^ $(CXXFLAGS)

reply_static_test:
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread reply_static_test.cc reply_static.cc mime_types.cc mime_types.h ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o reply_static_test -lboost_system

reply_echo_test:
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread reply_echo_test.cc reply_echo.cc mime_types.cc mime_types.h ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o reply_echo_test -lboost_system

reply_test: 
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread reply_test.cc reply.cc mime_types.cc mime_types.h ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o reply_test -lboost_system

request_handle_test:
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread request_handle_test.cc reply.cc reply.h request_handle.cc request_handle.h mime_types.cc reply_static.cc reply_echo.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o request_handle_test -lboost_system

webserver_test:
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread webserver_test.cc webserver.cc config_parser.cc reply.cc request_handle.cc mime_types.cc reply_static.cc reply_echo.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o webserver_test -lboost_system

config_parser_test:
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread config_parser_test.cc config_parser.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o config_parser_test

clean:
	rm -rf *.o *~ *.gch *.swp *.dSYM *.gcda *.gcno *.gcov config_parser config_parser_test *.tar.gz webserver reply.o request_handle.o reply_test webserver_test request_handle_test reply_echo_test reply_static_test

test: clean default
	./reply_test
	./webserver_test
	./request_handle_test
	./reply_echo_test
	./reply_static_test

integration_test: clean default
	./integration_test.sh
	
test_coverage:

	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread request_handle_test.cc reply.cc reply.h request_handle.cc mime_types.cc reply_static.cc reply_echo.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o request_handle_test -lboost_system -fprofile-arcs -ftest-coverage
	./request_handle_test; gcov -r request_handle.cc
	
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread webserver_test.cc webserver.cc config_parser.cc reply.cc request_handle.cc mime_types.cc reply_static.cc reply_echo.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o webserver_test -lboost_system -fprofile-arcs -ftest-coverage
	./webserver_test; gcov -r webserver.cc
  
  g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread webserver_test.cc webserver.cc config_parser.cc reply.cc request_handle.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o webserver_test -lboost_system -fprofile-arcs -ftest-coverage
	./reply_echo_test; gcov -r reply_echo.cc

	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread webserver_test.cc webserver.cc config_parser.cc reply.cc request_handle.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o webserver_test -lboost_system -fprofile-arcs -ftest-coverage

	./reply_static_test; gcov -r reply_static.cc

