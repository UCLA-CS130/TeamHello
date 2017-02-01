#include "webserver.h"
#include <string>
#include <memory>
#include <iostream>
#include <cstdlib>
#include "config_parser.h"
#include <utility>

using namespace boost;
using namespace boost::system;
using namespace boost::asio;

class session;

Server::Server(const configArguments& configArgs)
: io_service()
, acceptor(io_service, ip::tcp::endpoint(ip::tcp::v4(), configArgs.port))
{
    (this->acceptor).listen();
	this->configContent = configArgs;

    doAccept();
}

void Server::doAccept()
{
	std::shared_ptr<session> sesh = std::make_shared<session>(io_service);
    acceptor.async_accept(sesh->socket, [sesh, this](const error_code& accept_error)
    {
        if(!accept_error)
        {
            session::read_request(sesh);
        }
		doAccept();
    });
}

void Server::run()
{
	std::cout<<"Server is running!\n";
	io_service.run();
}

int Server::parseConfig(int argc, const char * argv[], configArguments& configArgs)
{	
	try
	{
		if (argc != 2)
		{
			std::cerr << "Usage ./webserver <path to config file>\n";
			return 1;
		}
		NginxConfigParser config_parser;
		NginxConfig config_out;

		if (config_parser.Parse(argv[1], &config_out)) 
		{
        	unsigned int tmpPort = std::stoi(config_out.statements_[0]->tokens_[1]);
			if (tmpPort > 65535 || tmpPort < 0)
			{
				std::cerr << "The port number " << tmpPort << " in config file is invalid.\n";
				return 2;
			}
			configArgs.port = (short unsigned int)tmpPort;
     	}
     	else 
		{
         	std::cerr << "Could not parse config file.\n";
         	return 3;
     	}
	}

	catch(std::exception& error) 
	{
        std::cerr<< "Exception: "<< error.what() << "\n";
		return 4;
   	}

	return 0;
}

      
      
