/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MiniServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lauragm <lauragm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 23:20:51 by lauragm           #+#    #+#             */
/*   Updated: 2026/11/15 18:40:07 by lauragm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MiniServer.hpp"

MiniServer::MiniServer(int pt, const std::string &pass): port(pt), password(pass), serverFd(-1){}

void MiniServer::InitServer()
{
	serverFd = socket(AF_INET, SOCK_STREAM, 0); //crea el socket de escucha (fd)
	//AF_INET hace referencia al protocolo IPv4, SOCK_STREAM hace referencia al protocolo TCP
	if(serverFd == -1)
	{
		std::cerr << "Error: creation of socket failed" << std::endl;
		return;
	}
	//setsockopt() para establecer ajustes

	sockaddr_in serverAddress; //dirección del socket, IPv4 + port
	std::memset(&serverAddress, 0, sizeof(serverAddress)); //inicializamos a 0 antes de rellenar la estructura
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port); //htons convierte el puerto en orden de bytes de red.
	serverAddress.sin_addr.s_addr = INADDR_ANY; 
	//INADDR_ANY se usa cuando no queremos vincular nuestro socket a ninguna IP concreta y en su lugar hacer que escuche todas las IPs disponibles.

	if(bind(serverFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) //vinculamos el socket a la dirección
	{
		std::cerr << "Error: bind the socket failed" << std::endl;
		close(serverFd);
		return;
	}
	if(listen(serverFd, SOMAXCONN) == -1) //convierte el socket a uno pasivo hasta que entren conexiones entrantes
	{
		std::cerr << "Error: waiting for connections failed" << std::endl;
		close(serverFd);
		return;
	}
	//SOMAXCONN es el máximo del sistema
	if(fcntl(serverFd, F_SETFL, O_NONBLOCK) == -1) //pone el socket en modo no bloqueante (como dice el subject)
	{
		std::cerr << "Error: non-blocking mode failed" << std::endl;
		close(serverFd);
		return;
	}//IMPORTANTE: este paso se debe de poner en cada socket de cliente

	//-----HASTA AQUÍ SE CREA EL SOCKET DE ESCUCHA-----
	struct pollfd serverPoll; //poll decide que hacer con los fds. El programa se duerme hasta que alguno tenga actividad (o pase el timeout), y al volver te dice cuáles
	serverPoll.fd = serverFd;
	serverPoll.events = POLLIN; //POLLIN: Hay algo que leer (en el socket de escucha: hay un cliente esperando)
	serverPoll.revents = 0; //lo rellena poll()

	pollFds.push_back(serverPoll); //con poll empieza el "motor" del servidor, aqui realizas una copia de la estructura al vector
}

MiniServer::~MiniServer()
{
	if(serverFd != -1)
		close(serverFd);
}