/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lauragm <lauragm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 18:24:12 by lauragm           #+#    #+#             */
/*   Updated: 2026/11/26 20:56:19 by lauragm          ###   ########.fr       */
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
		void appendToInput(const char *data, size_t len); 
		bool extractLine(std::string &line);
		void appendToOutput(const std::string &msg);
		bool hasOutput() const;
		const std::string &getOutputBuffer() const;
		void removeOutputBuffer(size_t n);
		
};

#endif