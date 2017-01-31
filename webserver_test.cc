#include <fstream>
#include <cstdio>
#include "gtest/gtest.h"
#include "config_parser.h"
#include "webserver.h"

TEST(configParseTest, correctConfig)
{
	configArguments configArgs;
	const char* argv[2] = {"./webserver", "SimpleConfig"};
	int errorCode = Server::parseConfig(2, argv, configArgs);
	EXPECT_EQ(errorCode, 0);
	EXPECT_EQ(configArgs.port, 8080);
}

TEST(configParseTest, formatWrongConfig)
{
	configArguments configArgs;
	std::ofstream configFile;
	configFile.open("formatWrongConfig");
	configFile << "server {;";
	configFile.close();
	const char* argv[2] = {"./webserver", "formatWrongConfig"};
	int errorCode = Server::parseConfig(2, argv, configArgs);
	remove("formatWrongConfig");
	EXPECT_EQ(errorCode, 3);
}

TEST(configParseTest, portNumberWrongConfig)
{
	configArguments configArgs;
	std::ofstream configFile;
	configFile.open("portNumberWrongConfig");
	configFile << "port 999999;";
	configFile.close();
	const char* argv[2] = {"./webserver", "portNumberWrongConfig"};
	int errorCode = Server::parseConfig(2, argv, configArgs);
	remove("portNumberWrongConfig");
	EXPECT_EQ(errorCode, 2);
}
