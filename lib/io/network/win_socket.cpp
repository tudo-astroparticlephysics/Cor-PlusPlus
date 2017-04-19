/* win_socket.cpp
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#ifdef _WIN32

#include "lib/io/network/socket.h"

#include <string>
#include <iostream>

#include <winsock.h>
#include <WinSock2.h>
#include <ws2tcpip.h>

#include <clocale>
#include <locale>
#include <codecvt>

namespace lib
{
	namespace io
	{
		namespace network
		{
			std::atomic<int> Socket::m_sock_counter = 0;
			std::mutex Socket::m_mutex;

			/// Converts error number in human readable text
			std::string convertWSAError(int error)
			{
				LPWSTR errString = NULL;

				int size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
					0,
					error,
					0,
					(LPWSTR)&errString,
					0,
					0);

				std::wstring message(errString, size);
				LocalFree(errString);


				using convert_type = std::codecvt_utf8<wchar_t>;
				std::wstring_convert<convert_type, wchar_t> converter;
				return converter.to_bytes(message);
			}






			/**
			*   Initialise WSA only if non socket was create before
			*   
			*/
			Socket::Socket()
			{							
				std::unique_lock<std::mutex> lock(m_mutex);
				int counter = m_sock_counter.fetch_add(1);			

				if (counter == 0)
				{
					WSADATA wsa;
					if (WSAStartup(MAKEWORD(2, 0), &wsa) != 0)		/// Startup WSA with version 2.0
					{
						std::cerr << "Failed. Error Code : " << WSAGetLastError() << std::endl;
					}
				}
				else
				{

				}

				memset(&m_addr, 0, sizeof(SOCKADDR_IN)); // zuerst alles auf 0 setzten 
			}

			Socket::Socket(Socket&& p_rhs)
			{
				this->m_sock = p_rhs.m_sock;
				this->m_addr = p_rhs.m_addr;

				m_sock_counter.fetch_add(1);

				p_rhs.m_sock = INVALID_SOCKET;
				memset(&p_rhs.m_addr, 0, sizeof(SOCKADDR_IN)); // zuerst alles auf 0 setzten 
			}

			Socket::~Socket()
			{
				if (m_sock != INVALID_SOCKET)
				{
					closesocket(m_sock);
				}


				if (m_sock_counter.fetch_sub(1) == 1)
				{
					WSACleanup();
				}
			}

			bool Socket::create()
			{
				m_sock = ::socket(AF_INET, SOCK_STREAM, 0);

				if (m_sock == INVALID_SOCKET)
				{
					std::cerr << "Fehler: Der Socket konnte nicht erstellt werden, error code: " << WSAGetLastError() << std::endl;
					return false;
				}
				return true;
			}

			bool Socket::close()
			{
				if (m_sock != INVALID_SOCKET)
				{
					long rc;
					rc = ::closesocket(m_sock);
					m_sock = INVALID_SOCKET;

					if (rc == SOCKET_ERROR)
					{
						std::cerr << "Fehler: close, error code: " << WSAGetLastError() << std::endl;
						return false;
					}
				}
				return true;
			}

			bool Socket::bind(const unsigned short p_port)
			{
				m_addr.sin_family = AF_INET;
				m_addr.sin_port = htons(p_port);
				m_addr.sin_addr.s_addr = ADDR_ANY;
				long rc = ::bind(m_sock, (SOCKADDR*)&m_addr, sizeof(SOCKADDR_IN));

				if (rc == SOCKET_ERROR)
				{
					std::cerr << "Error: bind, error code: " << WSAGetLastError() << std::endl;
					return false;
				}
				return true;
			}

			bool Socket::listen() const
			{
				long rc = ::listen(m_sock, 10);
				if (rc == SOCKET_ERROR)
				{
					std::cerr << "Error: listen, error code: " << WSAGetLastError() << std::endl;
					return false;
				}
				return true;
			}

			bool Socket::accept(Socket& p_new_socket) const
			{
				int size = sizeof(SOCKADDR_IN);
				p_new_socket.m_sock = ::accept(m_sock, (SOCKADDR*)&m_addr, &size);
				if (p_new_socket.m_sock == INVALID_SOCKET)
				{
					std::cerr << "Error: accept, error code: " << WSAGetLastError() << std::endl;
					return false;
				}
				return true;
			}

			bool Socket::send(const uint8_t* const p_data, unsigned int p_length)
			{
				long rc;

				rc = ::send(m_sock, reinterpret_cast<const char*>(p_data), p_length, 0);

				if (rc == SOCKET_ERROR)
				{
					std::cerr << "Fehler: send gescheitert, error code: " << WSAGetLastError() << std::endl;

					::closesocket(m_sock);
					m_sock = INVALID_SOCKET;
				}
				else
				{
					if (rc > 0 && rc < static_cast<long>(p_length) )
					{
						throw(std::exception("No complete message could be send"));						
					}
					return true;
				}

				return false;
			}

			std::vector<uint8_t> Socket::recv(bool p_noBlock)
			{
				std::vector<uint8_t> buf(1024);
				long rc;

				if (p_noBlock == true)
				{
					u_long iMode = 1;
					ioctlsocket(m_sock, FIONBIO, &iMode);
				}
				else
				{
					u_long iMode = 0;
					ioctlsocket(m_sock, FIONBIO, &iMode);
				}
				rc = ::recv(m_sock, reinterpret_cast<char*>(buf.data()), 1024, 0);

				if (rc == SOCKET_ERROR)
				{
					int nError = WSAGetLastError();
					if (nError != WSAEWOULDBLOCK && nError != 0)
					{
						std::cerr << "Error: recv, error code: " << nError << std::endl;
						::closesocket(m_sock);
						m_sock = INVALID_SOCKET;

						return std::vector<uint8_t>();
					}
				}
				else if (rc == 0)	/// connection was closed
				{
					return std::vector<uint8_t>();
				}
				else if (rc > 0)
				{
					buf.resize(rc);

					return buf;
				}

				return std::vector<uint8_t>();
			}

			bool Socket::connect(const std::string p_host, const unsigned short p_port)
			{
				m_addr.sin_family = AF_INET;
				m_addr.sin_port = htons(p_port);						/// set port

				inet_pton(m_addr.sin_family, p_host.c_str(), &m_addr.sin_addr.s_addr);  /// set host ip (deprecated inet_addr(host.c_str());)

				long rc = ::connect(m_sock, (SOCKADDR*)&m_addr, sizeof(SOCKADDR));
				if (rc == SOCKET_ERROR)
				{
					std::cerr << "Error: connect, error code: " << WSAGetLastError() << std::endl;
					return false;
				}

				return true;
			}

			bool Socket::is_valid() const
			{
				return m_sock != INVALID_SOCKET;
			}

			std::string Socket::getIP() const
			{
				char str[256];
				std::stringstream sstr;

				inet_ntop(m_addr.sin_family, (void*)(&(m_addr.sin_addr)), str, 256);

				sstr << str;
				sstr << ":" << ntohs(m_addr.sin_port);

				return sstr.str();
			}


			const std::vector<Socket*> Socket::waitFor_read(std::vector<Socket*> p_sock, const long long p_timeout_us)
			{
				FD_SET set;
				int rc;

				FD_ZERO(&set); // fd_set leeren

				for (auto itr : p_sock)
				{
					FD_SET(itr->m_sock, &set); // socket1 hinzur�fen		
				}

				if (p_timeout_us != 0)
				{
					timeval time;
					time.tv_sec = 0;
					time.tv_usec = static_cast<long>(p_timeout_us);


					rc = ::select(0, &set, NULL, NULL, &time);	/// Returns number of avaible sockets
				}
				else
				{
					rc = ::select(0, &set, NULL, NULL, NULL);	/// Returns number of avaible sockets
				}

				if (rc == SOCKET_ERROR)
				{
					std::cerr << "Fehler: select, error code: " << WSAGetLastError() << std::endl;
					return std::vector<Socket*>();
				}

				std::vector<Socket*> ret;
				for (auto itr : p_sock)
				{
					rc = FD_ISSET(itr->m_sock, &set); // Prüfen ob socket1 in diesem fd_set ist, rc ist in diesem falle TRUE (bzw grösser 0)
					if (rc > 0)
					{
						ret.push_back(itr);
					}

				}

				return std::move(ret);
			}

		}
	}
}
#endif
