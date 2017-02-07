#include "gtest/gtest.h"
#include "reply_static.h"
#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <utility>

TEST(ReplyStaticTest, GetResponse_notStatic)
{
	reply_static r;
	std::string s = "test string";
	std::stringstream ssOut;

	std::string sHTML = "<html><body><p>URL does not start with \"/static/\" </p></body></html>";
        ssOut << "HTTP/1.1 404 Not Found" << std::endl;
        ssOut << "content-type: text/html" << std::endl;
        ssOut << "content-length: " << sHTML.length() << std::endl;
        ssOut << std::endl;
        ssOut << sHTML;

	std::string b = "base";

	r.url = "/echo";

	std::string s2 = r.get_response(s,b);
	EXPECT_EQ(ssOut.str(),s2);
}

TEST(ReplyStaticTest, GetResponse_EmptyFile)
{
	reply_static r;
	std::string s = "test string";
	std::stringstream ssOut;

	std::string sHTML = "<html><body><p>URL starts with \"/static/\", but no file specified. </p></body></html>";
        ssOut << "HTTP/1.1 404 Not Found" << std::endl;
        ssOut << "content-type: text/html" << std::endl;
        ssOut << "content-length: " << sHTML.length() << std::endl;
        ssOut << std::endl;
        ssOut << sHTML;

	std::string b = "base";

	r.url = "/static/";

	std::string s2 = r.get_response(s,b);
	EXPECT_EQ(ssOut.str(),s2);
}

TEST(ReplyStaticTest, GetResponse_NonEmptyFile_exist)
{
	reply_static r;
	std::string s = "test string";
	std::stringstream ssOut;

	std::string content = "";
	std::string full_path = "./picture/apple.jpg";
      	mime_types mime;
	std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
	std::string extension = "jpeg";
      	char buf[512];
      	while (is.read(buf, sizeof(buf)).gcount() > 0)
        content.append(buf, is.gcount());

      	ssOut << "HTTP/1.1 200 OK" << std::endl;
     	ssOut << "content-type: " << mime.extension_to_type(extension) << std::endl;
      	ssOut << "content-length: " << content.length() << std::endl;
      	ssOut << std::endl;
      	ssOut << content;

	std::string b = "./picture/";

	r.url = "/static/apple.jpg";

	std::string s2 = r.get_response(s,b);
	EXPECT_EQ(ssOut.str(),s2);
}

TEST(ReplyStaticTest, GetResponse_NonEmptyFile_notexist)
{
	reply_static r;
	std::string s = "test string";
	std::stringstream ssOut;

	std::string content = "";
	std::string full_path = "./picture/orange.jpg";
      	mime_types mime;
	std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
	std::string extension = "jpeg";
      	char buf[512];
      	while (is.read(buf, sizeof(buf)).gcount() > 0)
        content.append(buf, is.gcount());

      	 std::string sHTML = "<html><body><p>File not found, can't open.</p></body></html>";
         ssOut << "HTTP/1.1 404 Not Found" << std::endl;
         ssOut << "content-type: text/html" << std::endl;
         ssOut << "content-length: " << sHTML.length() << std::endl;
         ssOut << std::endl;
         ssOut << sHTML;

	std::string b = "./picture/";

	r.url = "/static/orange.jpg";

	std::string s2 = r.get_response(s,b);
	EXPECT_EQ(ssOut.str(),s2);
}
