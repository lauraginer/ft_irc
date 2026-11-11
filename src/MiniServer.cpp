/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MiniServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lauragm <lauragm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 23:20:51 by lauragm           #+#    #+#             */
/*   Updated: 2026/11/11 23:20:52 by lauragm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MiniServer.hpp"

MiniServer::MiniServer(int pt, const std::string &pass): port(pt), password(pass), serverFd(-1)
{
	/*std::cout << pt << std::endl;
	std::cout << password << std::endl;*/
}

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

}

MiniServer::~MiniServer()
{
	if(serverFd != -1)
		close(serverFd);
}