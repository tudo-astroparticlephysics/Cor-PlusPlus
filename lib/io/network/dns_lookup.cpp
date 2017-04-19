/* dns_lookup.cpp
 * this file is part of Dynstack/RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#ifdef __linux__
#include <sys/socket.h>
#include <errno.h> //For errno - the error number
#include <netdb.h> //hostent
#include <netinet/in.h>
#include <arpa/inet.h>
#elif _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>

#include "lib/io/network/socket.h"
#endif

#include <string>
#include <cstring>

#include <iostream>
#include <stdexcept>



namespace lib
{
	namespace io
	{
		namespace network
		{
#ifdef __linux__
			std::string hostname_to_ip(const std::string p_hostname)
			{
				struct addrinfo hints;
				struct addrinfo* servinfo;

				memset(&hints, 0, sizeof(hints) );
				hints.ai_family = AF_UNSPEC; /// Allow IPv4 and IPv6
				hints.ai_socktype = SOCK_STREAM;

				int error = getaddrinfo(p_hostname.c_str(), NULL, &hints, &servinfo);

				if (error != 0)
				{
					std::cerr << "getaddrinfo: " << gai_strerror(error) << std::endl;
					throw( std::runtime_error( std::string("Could not identify host: ") + p_hostname) );
					return std::string() ;
				}

				std::string ip;

				// loop through all the results and connect to the first we can
				for (struct addrinfo* p = servinfo; p != NULL; p = p->ai_next)
				{
					struct sockaddr_in* h = (struct sockaddr_in *) p->ai_addr;
					ip = std::string(inet_ntoa(h->sin_addr));
					break;
				}

				freeaddrinfo(servinfo); // all done with this structure

				return ip;
			}
#elif _WIN32
		std::string hostname_to_ip(const std::string p_hostname)
		{
			Socket sock;	//does nothing more then WSAstartup();

			struct addrinfo hints;
			struct addrinfo* servinfo;

			memset(&hints, 0, sizeof(hints));
			hints.ai_family = AF_UNSPEC; /// Allow IPv4 and IPv6
			hints.ai_socktype = SOCK_STREAM;

			int error = getaddrinfo(p_hostname.c_str(), NULL, &hints, &servinfo);

			if (error != 0)
			{
				std::cerr << "getaddrinfo: " << gai_strerror(error) << std::endl;
				throw(std::runtime_error(std::string("Could not identify host: ") + p_hostname));				
			}

			std::string ip;

			// loop through all the results and connect to the first we can
			for (addrinfo* p = servinfo; p != NULL; p = p->ai_next)
			{
				struct sockaddr_in* h = (struct sockaddr_in *) p->ai_addr;
				ip = std::string(inet_ntoa(h->sin_addr));
				break;
			}

			freeaddrinfo(servinfo); // all done with this structure

			return ip;
			//return std::string();
		}
#else
#error System unsupported!
#endif
	}
}
}
