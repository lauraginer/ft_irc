/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MiniServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lauragm <lauragm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 23:20:51 by lauragm           #+#    #+#             */
/*   Updated: 2026/11/17 22:29:59 by lauragm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MiniServer.hpp"

MiniServer::MiniServer(int pt, const std::string &pass): port(pt), password(pass), serverFd(-1){}

void MiniServer::initServer()
{
	serverFd = socket(AF_INET, SOCK_STREAM, 0); //crea el socket de escucha (fd)
	//AF_INET hace referencia al protocolo IPv4, SOCK_STREAM hace referencia al protocolo TCP
	if(serverFd == -1)
		throw std::runtime_error("Error: creation of socket failed");

	//setsockopt() para establecer ajustes

	std::memset(&serverAddress, 0, sizeof(serverAddress)); //inicializamos a 0 antes de rellenar la estructura
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port); //htons convierte el puerto en orden de bytes de red.
	serverAddress.sin_addr.s_addr = INADDR_ANY; 
	//INADDR_ANY se usa cuando no queremos vincular nuestro socket a ninguna IP concreta y en su lugar hacer que escuche todas las IPs disponibles.

	if(bind(serverFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) //vinculamos el socket a la dirección
		throw std::runtime_error("Error: bind the socket failed");

	if(listen(serverFd, SOMAXCONN) == -1) //convierte el socket a uno pasivo hasta que entren conexiones entrantes
		std::runtime_error("Error: waiting for connections failed");

	//SOMAXCONN es el máximo del sistema
	if(fcntl(serverFd, F_SETFL, O_NONBLOCK) == -1) //pone el socket en modo no bloqueante (como dice el subject)
		throw std::runtime_error("Error: non-blocking mode failed");
	//IMPORTANTE: este paso se debe de poner en cada socket de cliente

	//-----HASTA AQUÍ SE CREA EL SOCKET DE ESCUCHA-----
	struct pollfd serverPoll; //poll decide que hacer con los fds. El programa se duerme hasta que alguno tenga actividad (o pase el timeout), y al volver te dice cuáles
	serverPoll.fd = serverFd;
	serverPoll.events = POLLIN; //POLLIN: Hay algo que leer (en el socket de escucha: hay un cliente esperando)
	serverPoll.revents = 0; //lo rellena poll()

	pollFds.push_back(serverPoll); //con poll empieza el "motor" del servidor, aqui realizas una copia de la estructura al vector
}

void MiniServer::runServer()
{
	while(true)
	{
		int ready = poll(&pollFds[0], pollFds.size(), -1); //llamada al so, el programa aparece dormido hasta que ocurra algo en los fds
		if(ready == -1) //ready es el numero de elementos de la lista con revents
			throw std::runtime_error("Error: poll failed");
		std::cout << "poll woke up, " << ready << " fds active" << std::endl;

		//monitoreo de la actividad
		size_t i = 0;
		while(i < pollFds.size()) //para comprobar cuales son los fds activos
		{
			if(pollFds[i].revents & POLLIN) //compruebas el bit de POLLIN si esta activado
			{
				if(pollFds[i].fd == serverFd)
					acceptClient(); //AQUI iria accept
				else
					std::cout << "Client fd " << pollFds[i].fd << " sent data" << std::endl;

			}
			i++;
		}
	}
}

void MiniServer::acceptClient() //creamos el socket de cliente con accept()
{
	std::cout << "Handling new connection..." << std::endl;
	
	int clientFd = accept(serverFd, NULL, NULL); //aceptamos conexion con el cliente, pasamos NULL porque de momento no necesitamos la IP del cliente
	if(clientFd == -1)
		std::cerr << "Error: The connection with client failed" << std::endl;

	//al hacer accept, la conexión sale de la cola y el bucle infinito de socket desaparece
	if(fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1) //pone el socket en modo no bloqueante (como dice el subject)
	{
		close(clientFd);
		std::cerr << "Error: could not set client non-blocking" << std::endl;
	}
	
	struct pollfd clientPoll; //creamos de nuevo la estructura para que poll la pueda manejar
	clientPoll.fd = clientFd;
	clientPoll.events = POLLIN;
	clientPoll.revents = 0;

	pollFds.push_back(clientPoll);
	
	std::cout << "New client connected: fd " << clientFd << std::endl;
}

MiniServer::~MiniServer()
{
	if(serverFd != -1)
		close(serverFd);
}