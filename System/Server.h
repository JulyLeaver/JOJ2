#pragma once

#include <cstring>
#include <string>
#include <map>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <thread>
#include <mutex>
#include "Logger.h"

#define HEADER_MSG_SIZE 1024
#define HEADER_BUF_SIZE 1048576 // 1mb
struct PACKET
{
	char msg[HEADER_MSG_SIZE];
	char buf[HEADER_BUF_SIZE];
};

class Server
{
private:
	int server_fd;
	bool server_down;
	
	std::thread _thread;
	std::map<std::string, std::pair<int, struct sockaddr_in>> user_fd; // 유저id에 대한 소켓 파일 디스크립터

	Logger* logger;

	PACKET* infoPacket;
	PACKET* errorPacket;

public:
	Server(Logger* logger);
	~Server();

	void start();
	void stop();

private:
	void serverLoop();

	inline bool addUser(const std::string& userId, const int client_fd, const struct sockaddr_in& client_in)
	{
		if (user_fd.find(userId) != user_fd.end()) return false;
		user_fd.insert({ userId, { client_fd, client_in } });
	}

};