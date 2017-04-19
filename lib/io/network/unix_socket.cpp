/* unix_socket.cpp
 * this file is part of Dynstack/RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#ifdef __linux__

#include "lib/io/network/socket.h"

#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>	// close, gethostname
#include <arpa/inet.h>	//INET_PTON

#include <cstring>
#include <iostream>
#include <vector>

namespace lib
{
	namespace io
	{
		namespace network
		{
			Socket::Socket()
					: m_sock(-1)
			{

				memset(&m_addr, 0, sizeof(m_addr));

			}

			Socket::Socket(Socket&& p_rhs)
			{
				this->m_addr = p_rhs.m_addr;
				memset(&p_rhs.m_addr, 0, sizeof(sockaddr_in));

				this->m_sock = p_rhs.m_sock;
				p_rhs.m_sock = 0;
			}

			Socket::~Socket()
			{
				if (this->is_valid())
				{
					::close(m_sock);
					m_sock = 0;
				}
			}

			bool Socket::create()
			{
				m_sock = socket(AF_INET, SOCK_STREAM, 0);

				if (!is_valid())
					return false;

				// TIME_WAIT - argh
				int on = 1;
				if (setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (const char*) &on, sizeof(on)) == -1)
					return false;
				else
					return true;
			}

			bool Socket::close()
			{
				if (this->is_valid())
				{
					::close(m_sock);
					m_sock = 0;
					return true;
				}
				return false;
			}

			bool Socket::bind(const unsigned short p_port)
			{

				if (!is_valid())
				{
					return false;
				}

				m_addr.sin_family = AF_INET;
				m_addr.sin_addr.s_addr = INADDR_ANY;
				m_addr.sin_port = htons(p_port);

				int bind_return = ::bind(m_sock, (struct sockaddr *) &m_addr, sizeof(m_addr));

				if (bind_return == -1)
				{
					return false;
				}

				return true;
			}

			bool Socket::listen() const
			{
				if (!is_valid())
				{
					return false;
				}

				int listen_return = ::listen(m_sock, 128);	// Second parameter = number of waiting clients in queue

				if (listen_return == -1)
				{
					return false;
				}

				return true;
			}

			bool Socket::accept(Socket& p_new_socket) const
			{
				int addr_length = sizeof(sockaddr_in);
				p_new_socket.m_sock = ::accept(m_sock, (sockaddr *) &(p_new_socket.m_addr), (socklen_t *) &addr_length);

				if (p_new_socket.m_sock <= 0)
					return false;
				else
					return true;
			}

			bool Socket::send(const u_int8_t* const p_data, unsigned int p_length)
			{
				int status = ::send(m_sock, p_data, p_length, MSG_NOSIGNAL);
				if (status == -1)
				{
					return false;
				}
				else
				{
					return true;
				}
			}

			std::vector<u_int8_t> Socket::recv(bool p_noBlock)
			{
				std::vector<u_int8_t> buf(1024);

				int flags = p_noBlock ? MSG_DONTWAIT : 0;
				int status = ::recv(m_sock, buf.data(), 1024, flags);

				if (p_noBlock)
				{
					if (errno == EAGAIN || errno == EWOULDBLOCK)
						return std::vector<u_int8_t>();
				}

				if (status == -1)
				{
					if (errno != EAGAIN && errno != EWOULDBLOCK)
						std::cout << "status == -1   errno == " << errno << "  in Socket::recv\n";

					return std::vector<u_int8_t>();
				}
				else if (status == 0)	/// Connection closed
				{
					return std::vector<u_int8_t>();
				}
				else
				{
					buf.resize(status);
					return buf;
				}
			}

			bool Socket::connect(const std::string p_host, const unsigned short p_port)
			{
				if (!is_valid())
				{
					std::cerr << __LINE__ << " Invalide socket before connect" << std::endl;
					return false;
				}

				m_addr.sin_family = AF_INET;
				m_addr.sin_port = htons(p_port);

				int status = inet_pton(AF_INET, p_host.c_str(), &m_addr.sin_addr);

				if ( errno == EAFNOSUPPORT)
				{
					std::cerr << __LINE__ << " Could not convert string ip to binary format" << std::endl;
					return false;
				}

				status = ::connect(m_sock, (sockaddr *) &m_addr, sizeof(m_addr));

				if (status < 0)
				{
					std::cerr << __LINE__ << " Could not connect to ip" << std::endl;
					return false;
				}
				return true;
			}

			bool Socket::is_valid() const
			{
				return m_sock != 0;
			}

			std::string Socket::getIP() const
			{
				std::stringstream sstr;

				char str[256];
				inet_ntop(AF_INET, &(m_addr.sin_addr), str, 256);

				sstr << str;
				sstr << ":" << ntohs(m_addr.sin_port);

				return sstr.str();
			}

			const std::vector<Socket*> Socket::waitFor_read(std::vector<Socket*> p_sock, const long long p_timeout_us)
			{
				int rc;

				if (p_sock.size() >= Socket::waitFor_size)
				{
					std::cerr << "Maximal size for waitFor is: " << FD_SETSIZE << std::endl;
					throw(std::out_of_range("waitFor size to big"));
				}

				fd_set set;
				FD_ZERO(&set);

				int maxfds = 0;

				for (auto itr : p_sock)
				{
					FD_SET(itr->m_sock, &set); // socket hinzufügen

					if (maxfds < itr->m_sock)
						maxfds = itr->m_sock;
				}

				if (p_timeout_us != 0)
				{
					timeval time;
					time.tv_sec = 0;
					time.tv_usec = p_timeout_us;

					rc = ::select(maxfds + 1, &set, (fd_set*) NULL, (fd_set*) NULL, &time);	/// Returns number of avaible sockets
				}
				else
				{
					rc = ::select(maxfds + 1, &set, (fd_set*) NULL, (fd_set*) NULL, (timeval*) NULL);	/// Returns number of avaible sockets
				}

				if (rc < 0)
				{
					std::cerr << "Error: select, error code: " << strerror(errno) << std::endl;
					return std::vector<Socket*>();
				}

				std::vector<Socket*> ret;
				for (auto itr : p_sock)
				{
					rc = FD_ISSET(itr->m_sock, &set); // Test if socket is in fd_set, rc is TRUE (> 0)
					if (rc > 0)
					{
						ret.push_back(itr);
					}

				}

				return ret;

			}

			const std::vector<Socket*> Socket::waitFor_write(std::vector<Socket*> p_sock, const long long p_timeout_us)
			{
				int rc;

				if (p_sock.size() >= Socket::waitFor_size)
				{
					std::cerr << "Maximal size for waitFor is: " << FD_SETSIZE << std::endl;
					throw(std::out_of_range("waitFor size to big"));
				}

				fd_set set;
				FD_ZERO(&set);

				int maxfds = 0;

				for (auto itr : p_sock)
				{
					FD_SET(itr->m_sock, &set); // socket hinzufügen

					if (maxfds < itr->m_sock)
						maxfds = itr->m_sock;
				}

				if (p_timeout_us != 0)
				{
					timeval time;
					time.tv_sec = 0;
					time.tv_usec = p_timeout_us;

					rc = ::select(maxfds + 1, (fd_set*) NULL, &set, (fd_set*) NULL, &time);	/// Returns number of avaible sockets
				}
				else
				{
					rc = ::select(maxfds + 1, (fd_set*) NULL, &set, (fd_set*) NULL, (timeval*) NULL);	/// Returns number of avaible sockets
				}

				if (rc < 0)
				{
					std::cerr << "Error: select, error code: " << strerror(errno) << std::endl;
					return std::vector<Socket*>();
				}

				std::vector<Socket*> ret;
				for (auto itr : p_sock)
				{
					rc = FD_ISSET(itr->m_sock, &set); // Test if socket is in fd_set, rc is TRUE (> 0)
					if (rc > 0)
					{
						ret.push_back(itr);
					}

				}

				return ret;

			}
		}
	}
}

#endif

