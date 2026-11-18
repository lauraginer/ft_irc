/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MiniServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lauragm <lauragm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 23:20:59 by lauragm           #+#    #+#             */
/*   Updated: 2026/11/18 19:53:22 by lauragm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISERVER_HPP
#define MINISERVER_HPP

#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <vector>  

class MiniServer{

	private:
		int	port;
		std::string password;
		int serverFd;
		sockaddr_in serverAddress; //dirección del socket, IPv4 + port
		std::vector<struct pollfd>	pollFds;

	public:
		MiniServer(int pt, const std::string &pass);
		void initServer();
		void runServer();
		void acceptClient();
		bool readClient(size_t index);
		~MiniServer();
};

#endif