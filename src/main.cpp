/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lauragm <lauragm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 20:04:21 by lauragm           #+#    #+#             */
/*   Updated: 2026/11/05 22:26:26 by lauragm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>

int main(int argc, char **argv)
{
	if(argc != 3)
	{
		std::cerr << "Usage: ./ircserver_bonus <port> <password>\n";
		return(1);
	}
	std::string port(argv[1]);
	std::string password(argv[2]);
	if(port.empty() && password.empty())
	{
		std::cerr << "Usage: ./ircserver_bonus <port> <password>\n";
		return(1);
	}
	if(port.empty())
	{
		//comprobar basicos, carácteres numericos y aceptar los ports que no esten reservados
		std::cerr << ":Port not valid. Enter a number between 1024 and 65535 \n";
		return(1);
	}
	if(password.empty() || (password.size() < 4) || password[0] == ' ')
	{
		std::cerr << ":Password incorrect\n";
		return(464);
	}
		
		
	
}