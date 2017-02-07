#include "gtest/gtest.h"
//#include "gmock/gmock.h"
#include "request_handle.h"
#include <iostream>
#include <sstream>

using namespace boost;
using namespace boost::system;
using namespace boost::asio;

class session;

class RequestTest : public testing::Test {

    protected:



};

TEST_F(RequestTest, read_next_line_text)
{
	io_service io_service;
   	ip::tcp::endpoint endpoint{ip::tcp::v4(), 8080};
	std::shared_ptr<session> sesh = std::make_shared<session>(io_service, "/home/bob", "/echo", "/static");

	std::string line, ignore;
	std::istringstream ss("Hello\r\n");
	std::istringstream ss1("Hello\r\n\r\n");
	//std::istream stream {&ss};

	std::string test, test1;
	test += sesh->read_next_line_text(ss, line, ignore);
	test1 += sesh->read_next_line_text(ss1, line, ignore);

	EXPECT_EQ( test, "Hello\r\n");
	EXPECT_EQ( test1, "Hello\r\n");
}


TEST_F(RequestTest, read_first_line_text)
{
	io_service io_service;
   	ip::tcp::endpoint endpoint{ip::tcp::v4(), 8080};
	std::shared_ptr<session> sesh = std::make_shared<session>(io_service, "/home/bob", "/echo", "/static");

	std::string line, ignore;
	std::istringstream ss("Hello\r\n");
	std::istringstream ss1("Hello\r\n\r\n");
	//std::istream stream {&ss};

	std::string test, test1;
	test += sesh->read_first_line_text(ss, line, ignore);
	test1 += sesh->read_first_line_text(ss1, line, ignore);

	EXPECT_EQ( test, "Hello\r\n");
	EXPECT_EQ( test1, "Hello\r\n");
}




TEST_F(RequestTest, read_request)
{
	io_service io_service;
   	ip::tcp::endpoint endpoint{ip::tcp::v4(), 8080};
   	ip::tcp::acceptor acceptor{io_service, endpoint};
        std::shared_ptr<session> sesh = std::make_shared<session>(io_service, "/home/bob", "/echo", "/static");
	io_service.run();

	char data[] = "Hello\r\n";

	//doesn't really write to socket since it is on same side as read.
        asio::async_write(sesh->socket, asio::buffer(data, 7) ,[sesh](const error_code& e, std::size_t s)
      	{ });

	sesh->read_request(sesh);	

	//Since socket is empty, string ss is not augmented
	EXPECT_EQ(sesh->ss, "");
}

TEST_F(RequestTest, read_first_line)
{
	io_service io_service;
   	ip::tcp::endpoint endpoint{ip::tcp::v4(), 8080};
   	ip::tcp::acceptor acceptor{io_service, endpoint};
        std::shared_ptr<session> sesh = std::make_shared<session>(io_service, "/home/bob", "/echo", "/static");

	io_service.run();

	char data[] = "Hello\r\n";

	//doesn't really write to socket since it is on same side as read.
        asio::async_write(sesh->socket, asio::buffer(data, 7) ,[sesh](const error_code& e, std::size_t s)
      	{ });

	sesh->read_first_line(sesh);	

	//Since socket is empty, string ss is not augmented
	EXPECT_EQ(sesh->ss, "");
}

TEST_F(RequestTest, read_next_line)
{
	io_service io_service;
   	ip::tcp::endpoint endpoint{ip::tcp::v4(), 8080};
   	ip::tcp::acceptor acceptor{io_service, endpoint};
        std::shared_ptr<session> sesh = std::make_shared<session>(io_service, "/home/bob", "/echo", "/static");

	io_service.run();

	char data[] = "Hello\r\n";

	//doesn't really write to socket since it is on same side as read.
        asio::async_write(sesh->socket, asio::buffer(data, 7) ,[sesh](const error_code& e, std::size_t s)
      	{ });

	sesh->read_next_line(sesh);	

	//Since socket is empty, string ss is not augmented
	EXPECT_EQ(sesh->ss, "");
}


