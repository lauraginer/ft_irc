/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lauragm <lauragm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 20:04:21 by lauragm           #+#    #+#             */
/*   Updated: 2026/11/08 21:26:57 by lauragm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <stdexcept>

void parse_arguments(std::string port, std::string password)
{
	if(port.empty() && password.empty())
		throw std::runtime_error("wrong arguments. Usage: ./ircserver_bonus <port> <password>");
	//validacion del puerto
	if(port.empty() || port.size() > 5)
		throw std::runtime_error("Port not valid. Enter a number between 1024 and 65535");
	else
	{
		size_t i = 0;
		while(i < port.size())
		{
			if(!(isdigit(static_cast<unsigned char>(port[i])))) //casteo para evitar comportamientos indefinidos
				throw std::runtime_error("Port not valid. Enter a number between 1024 and 65535");
			i++;
		}
		int number = atoi(port.c_str());
		if(number < 1024 || number > 65535) //aceptar los ports que no esten reservados
			throw std::runtime_error("Port not valid. Enter a number between 1024 and 65535");

	}
	//validacion de la contraseña
	if(password.empty() || (password.size() < 4) || password[0] == ' ' || password[password.size() - 1] == ' ')
		throw std::runtime_error("Password incorrect");
}

int main(int argc, char **argv)
{
	try
	{
		if(argc != 3)
			throw std::runtime_error("wrong arguments. Usage: ./ircserver_bonus <port> <password>");
		parse_arguments(argv[1], argv[2]); 
		
		//las excepciones mejor solo para el parseo y algo minimo, no para el servidor
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return(1);
	}
	return(0);
}