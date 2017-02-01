//Inspired by Boost example for async_client.cpp and its use of async_read_until
//www.boost.org/doc/libs/1_43_0/doc/html/boost_asio/example/http/client/async_client.cpp

#include <boost/asio.hpp>
#include <string>
#include <memory>
#include <iostream>
#include <cstdlib>
#include <utility>
#include "request_handle.h"

using namespace boost;
using namespace boost::system;
using namespace boost::asio;

 
void session::read_body(std::shared_ptr<session> pThis)
   {
      int nbuffer = 1000;
      std::shared_ptr<std::vector<char>> buffer_ptr = std::make_shared<std::vector<char>>(nbuffer);
      //reads the body of the incoming request.
      asio::async_read(pThis->socket, boost::asio::buffer(*buffer_ptr, nbuffer), [pThis](const error_code& e, std::size_t s)
      {
      });
   }
   
void session::read_next_line(std::shared_ptr<session> pThis)
   {
      //reads the next line of an incoming request
      asio::async_read_until(pThis->socket, pThis->buff, '\r', [pThis](const error_code& e, std::size_t s)
      {
         std::string line, ignore;
         std::istream stream {&pThis->buff};

         pThis->ss += pThis->read_next_line_text(stream, line, ignore);
	 pThis->headers.read_header(line);
	 std::string echoback = pThis->ss;
         
         if(line.length() != 0)
         {
            pThis->read_next_line(pThis);
         }
         //This signals end of request
         else if(line.length() == 0)
         {
            if(pThis->headers.content_length() != 0)
            {
               pThis->read_body(pThis);
            }
            //This signals end of request's body
            else
            {
               std::shared_ptr<std::string> str = std::make_shared<std::string>(pThis->headers.get_response(echoback));
               asio::async_write(pThis->socket, boost::asio::buffer(str->c_str(), str->length()), [pThis, str](const error_code& e, std::size_t s)
               {
                  //std::cout << "done" << std::endl;
               });
            }
         }
      });
   }

std::string session::read_next_line_text(std::istream & stream, std::string & line, std::string & ignore)
   {
      	std::getline(stream, line, '\r');
        std::getline(stream, ignore, '\n');
        
        std::string current_line;
	current_line += line + "\r\n";      

	return current_line; 
   }

void session::read_first_line(std::shared_ptr<session> pThis)
   {
      asio::async_read_until(pThis->socket, pThis->buff, '\r', [pThis](const error_code& e, std::size_t s)
      {
         std::string line, ignore;
         std::istream stream {&pThis->buff};
 
	 pThis->ss += pThis->read_first_line_text(stream, line, ignore);
         pThis->read_next_line(pThis);
      });
      
   }

std::string session::read_first_line_text(std::istream & stream, std::string & line, std::string & ignore)
   {
      	std::getline(stream, line, '\r');
        std::getline(stream, ignore, '\n');
        //pThis->headers.read_request_line(line);
        std::string first_line;
        first_line += line + "\r\n";

	return first_line; 
   }
   

   
void session::read_request(std::shared_ptr<session> pThis)
   {
      pThis->read_first_line(pThis);
   }

