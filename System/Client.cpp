#include "Client.h"

Client::Client(Logger* logger, uint16_t port, const char* IP) :
	logger(logger),
	PORT(port), IP(IP)
{
	buff = new char[BUFF_SIZE];
}

bool Client::connectServer()
{
	sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0)
	{
		logger->log("create file descriptor fail");
		return false;
	}

	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(IP.c_str());
	if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
	{
		logger->log("connect server fail");
		return false;
	}
	return true;
}

TextMsgArray Client::readMsg()
{
	memset(buff, 0, sizeof(BUFF_SIZE));
	read(sockfd, buff, BUFF_SIZE - 2);
	return split(buff, ' ');
}

void Client::disconnect()
{
	close(sockfd);
}

Client::~Client()
{
	delete[]buff;
}
