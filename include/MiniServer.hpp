/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MiniServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lauragm <lauragm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 23:20:59 by lauragm           #+#    #+#             */
/*   Updated: 2026/11/11 23:21:02 by lauragm          ###   ########.fr       */
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