/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MiniServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lauragm <lauragm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 23:20:59 by lauragm           #+#    #+#             */
/*   Updated: 2026/11/24 19:36:43 by lauragm          ###   ########.fr       */
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
#include <map>
#include <csignal>

#include "Client.hpp"

extern volatile sig_atomic_t g_running;
void handleSignal(int);

class MiniServer{

	private:
		int	port;
		std::string password;
		int serverFd;
		sockaddr_in serverAddress; //dirección del socket, IPv4 + port
		std::vector<struct pollfd>	pollFds;
		std::map<int, Client> clients;

	public:
		MiniServer(int pt, const std::string &pass);
		~MiniServer();

		void initServer();
		void runServer();
		void acceptClient();
		bool readClient(size_t index);
};

#endif