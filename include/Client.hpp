/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lauragm <lauragm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 18:24:12 by lauragm           #+#    #+#             */
/*   Updated: 2026/11/24 21:16:16 by lauragm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client{
	private:
		int fd;
		std::string inputBuffer;
		std::string outputBuffer; //el buffer de salida que utilizaremos en send()
		
	public:
		Client();
		Client(int fd);
		~Client();

		int getFd() const;
		void appendToBuffer(const char *data, size_t len); 
		bool extractLine(std::string &line);
};

#endif