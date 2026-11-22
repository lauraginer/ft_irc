/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lauragm <lauragm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 18:24:08 by lauragm           #+#    #+#             */
/*   Updated: 2026/11/22 20:49:08 by lauragm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(): fd(-1){} //necesario para la creacion del std::map

Client::Client(int fd): fd(fd){}

Client::~Client(){}

int Client::getFd() const
{
	return(fd);
}

void Client::appendToBuffer(const char *data, size_t len) 
{
	inputBuffer.append(data, len); //append() para añadir al buffer
}
bool Client::extractLine(std::string &line)
{
	size_t pos = inputBuffer.find('\n');
	if(pos == std::string::npos)
		return(false);
	
	line = inputBuffer.substr(0, pos); //linea extraida hasta pos
	inputBuffer.erase(0, pos + 1);
	
	if(!line.empty() && line[line.size() - 1] == '\r')
		line.erase(line.size() - 1); //quitamos ese \r invisible que se queda en el texto y rompe cosas como el nick
	return(true);
	
}
