/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MiniServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lauragm <lauragm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 23:20:51 by lauragm           #+#    #+#             */
/*   Updated: 2026/11/22 22:22:58 by lauragm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MiniServer.hpp"

MiniServer::MiniServer(int pt, const std::string &pass): port(pt), password(pass), serverFd(-1){}

MiniServer::~MiniServer()
{
	if(serverFd != -1)
		close(serverFd);
}

void MiniServer::initServer()
{
	serverFd = socket(AF_INET, SOCK_STREAM, 0); //crea el socket de escucha (fd)
	//AF_INET hace referencia al protocolo IPv4, SOCK_STREAM hace referencia al protocolo TCP
	if(serverFd == -1)
		throw std::runtime_error("creation of socket failed");

	int opt = 1; //setsockopt() para establecer ajustes
	if(setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		throw std::runtime_error("setsockopt failed");

	std::memset(&serverAddress, 0, sizeof(serverAddress)); //inicializamos a 0 antes de rellenar la estructura
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port); //htons convierte el puerto en orden de bytes de red.
	serverAddress.sin_addr.s_addr = INADDR_ANY; 
	//INADDR_ANY se usa cuando no queremos vincular nuestro socket a ninguna IP concreta y en su lugar hacer que escuche todas las IPs disponibles.

	if(bind(serverFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) //vinculamos el socket a la dirección
		throw std::runtime_error("bind the socket failed");

	if(listen(serverFd, SOMAXCONN) == -1) //convierte el socket a uno pasivo hasta que entren conexiones entrantes
		std::runtime_error("waiting for connections failed");

	//SOMAXCONN es el máximo del sistema
	if(fcntl(serverFd, F_SETFL, O_NONBLOCK) == -1) //pone el socket en modo no bloqueante (como dice el subject)
		throw std::runtime_error("non-blocking mode failed");
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
			throw std::runtime_error("poll failed");
		std::cout << "poll woke up, " << ready << " fds active" << std::endl;

		//monitoreo de la actividad
		size_t i = 0;
		while(i < pollFds.size()) //para comprobar cuales son los fds activos
		{
			if(pollFds[i].revents & POLLIN) //compruebas el bit de POLLIN si esta activado
			{
				if(pollFds[i].fd == serverFd)
					acceptClient(); //accept
				else if(readClient(i)) //recv
					continue;
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
	clients[clientFd] = Client(clientFd); //asociamos el fd a su cliente correspondiente y creamos el mapa
	
	std::cout << "New client connected: fd " << clientFd << std::endl;
}

bool MiniServer::readClient(size_t index)
{
	std::cout << "Handling data from client..." << std::endl;
	char buffer[512];
	int fd = pollFds[index].fd; //guardamos el fd, ya que despues de erase el fd no existe y puede crashear

	ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1, 0); // -1 para el char nulo
	if(bytes <= 0) //si es -1 es error, si es 0 es desconectado, pero lo trartamos asi por obligacion del subject (prohibido consultar errno tras recv)
	{
		std::cout << "Client fd" << fd << " disconnected" << std::endl;
		close(fd);
		clients.erase(fd); //sincronizamos el mapa también
		pollFds.erase(pollFds.begin() + index);
		return(true);
	}
	/*buffer[bytes] = '\0'; //añadimos el nulo, ya que recv lee bytes crudos
	std::cout << "Received from fd " << fd << ": " << buffer;*/

	Client &client = clients[fd]; //el & es para modificar el cliente del mapa, no una copia
	client.appendToBuffer(buffer, bytes);
	
	std::string line;
	while(client.extractLine(line))
		std::cout << "Received from fd " << fd << ": [" << line << "]" << std::endl;

	return(false);
}

//Como el protocolo de IRC necesita líneas completas terminadas en \n o \r, debemos de crear un buffer por cliente

