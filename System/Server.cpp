#include <cstring>
#include <sys/select.h>
#include <sys/time.h>
#include "Server.h"
#include "util.h"

Server::Server(Logger* logger) : logger(logger), server_down(false)
{
	infoPacket = new PACKET;
	memset(infoPacket, 0, sizeof(infoPacket));
	strcpy(infoPacket->msg, "info");
	strcpy(infoPacket->buf, "C:C++ 9998:9999");

	errorPacket = new PACKET;
	memset(errorPacket, 0, sizeof(errorPacket));
	strcpy(errorPacket->msg, "smsg");
	strcpy(errorPacket->buf, "1");
}

Server::~Server()
{
	delete infoPacket;
	delete errorPacket;
}

void Server::start()
{
	server_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in server_in;
	memset(&server_in, 0, sizeof(sockaddr_in));
	server_in.sin_family = AF_INET;
	server_in.sin_port = htons(12345);
	server_in.sin_addr.s_addr = inet_addr("127.0.0.1");
	bind(server_fd, (struct sockaddr*)&server_in, sizeof(server_in));

	listen(server_fd, 5);

	_thread = std::thread(&Server::serverLoop, this);
}

void Server::stop()
{
	server_down = true;
	_thread.join();
}

void Server::serverLoop()
{
	struct sockaddr_in client_in;
	socklen_t adr_size = sizeof(client_in);

	struct timeval timeout;
	fd_set reads, cp_reads;

	FD_ZERO(&reads);
	FD_SET(server_fd, &reads);

	int fd_max = server_fd, fd_num;

	PACKET* packet = new PACKET;

	while (!server_down)
	{
		logger->log("server");
		cp_reads = reads;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		if ((fd_num = select(fd_max + 1, &cp_reads, 0, 0, &timeout)) == -1)
		{
			logger->log("server loop error");
			break;
		}
		if (fd_num == 0) continue;

		for (int i = 0; i < fd_max + 1; ++i)
		{
			if (FD_ISSET(i, &cp_reads))
			{
				if (i == server_fd)
				{
					int client_fd = accept(server_fd, (struct sockaddr*)&client_in, &adr_size);
					write(client_fd, infoPacket, sizeof(PACKET));
				
					FD_SET(client_fd, &reads);
					if (fd_max < client_fd) fd_max = client_fd;

				}
			}
			else
			{
			//	int len = read(i, packet, sizeof(packet));
			//	logger->log(packet->msg);
			}
		}
	}

	delete packet;
}