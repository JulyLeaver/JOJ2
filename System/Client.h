#pragma once

#include <cstring>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "Logger.h"
#include "util.h"

using std::vector;
using std::string;

using TextMsgArray = vector<string>;

class Client
{
private:
	Logger* logger;

	const uint16_t PORT;
	const string IP;

	int sockfd;
	struct sockaddr_in addr;

	const int BUFF_SIZE = 1024;
	char* buff;
public:
	Client(Logger* logger, uint16_t port, const char* IP);

	bool connectServer();
	TextMsgArray readMsg();

	void disconnect();
	~Client();
};

