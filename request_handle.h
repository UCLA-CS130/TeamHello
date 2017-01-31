#ifndef HTTP_REQUEST_HANDLER_H
#define HTTP_REQUEST_HANDLER_H

#include <boost/asio.hpp>
#include <string>
#include <memory>
#include <iostream>
#include <cstdlib>
#include <utility>
#include "request_handle.h"
#include "reply.h"

using namespace boost;
using namespace boost::system;
using namespace boost::asio;

// A struct to hold config arguments
struct configArguments
{
   short unsigned int port;
};


/// The common handler for all incoming requests.

class session
{
   asio::streambuf buff;
   http_headers headers;
   
   static void read_body(std::shared_ptr<session> pThis);
   
   static void read_next_line(std::shared_ptr<session> pThis);
   
   static void read_first_line(std::shared_ptr<session> pThis);
   
public:

   char data[4096];

   size_t length;

   std::string ss = "";

   ip::tcp::socket socket;
   
   session(io_service& io_service)
      :socket(io_service)
   {
   }
   
   static void read_request(std::shared_ptr<session> pThis);
};


#endif // HTTP_REQUEST_HANDLER_H
