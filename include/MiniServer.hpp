#ifndef MINISERVER_HPP
#define MINISERVER_HPP

#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

class MiniServer{

	private:
		int	port;
		std::string password;
		int serverFd;

	public:
		MiniServer(int pt, const std::string &pass);
		void InitServer();
		~MiniServer();
};

#endif