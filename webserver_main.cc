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

int main(int argc, const char * argv[])
{
   configArguments configArgs;
  
   int configParserErrorCode = Server::parseConfig(argc, argv, configArgs);
   if (configParserErrorCode != 0)
   {
      return configParserErrorCode;	
   }

   Server server(configArgs);
   
   server.run();
   return 0;

}
