/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lauragm <lauragm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 20:04:21 by lauragm           #+#    #+#             */
/*   Updated: 2026/11/08 20:56:13 by lauragm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <stdexcept>

bool parse_arguments(std::string port, std::string password)
{
	if(port.empty() && password.empty())
	{
		std::cerr << "Usage: ./ircserver_bonus <port> <password>\n";
		return(false);
	}
	//validacion del puerto
	if(port.empty() || port.size() > 5)
	{
		std::cerr << "Error: Port not valid. Enter a number between 1024 and 65535\n";
		return(false);
	}
	else
	{
		size_t i = 0;
		while(i < port.size())
		{
			if(!(isdigit(static_cast<unsigned char>(port[i])))) //casteo para evitar comportamientos indefinidos
			{
				std::cerr << "Error: Port not valid. Enter a number between 1024 and 65535\n";
				return(false);
			}
			i++;
		}
		int number = atoi(port.c_str());
		if(number < 1024 || number > 65535) //aceptar los ports que no esten reservados
		{
			std::cerr << "Error: Port not valid. Enter a number between 1024 and 65535\n";
			return(false);
		}
	}
	//validacion de la contraseña
	if(password.empty() || (password.size() < 4) || password[0] == ' ' || password[password.size() - 1] == ' ')
	{
		std::cerr << "Error: Password incorrect\n";
		return(false);
	}
	return(true);
}

int main(int argc, char **argv)
{
	if(argc != 3)
	{
		std::cerr << "Usage: ./ircserver_bonus <port> <password>\n";
		return(1);
	}
	if(!(parse_arguments(argv[1], argv[2])))
		return(1);
	return(0);
}