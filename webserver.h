#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <boost/asio.hpp>
#include "request_handle.h"

class Server
{
public:
	Server(const configArguments& configArgs);
	static int parseConfig(int argc, const char * argv[], configArguments& configArgs);
	void run();
private:
	void doAccept();

	boost::asio::io_service io_service;
	boost::asio::ip::tcp::acceptor acceptor;
	configArguments configContent;
};

#endif //WEBSERVER_H
