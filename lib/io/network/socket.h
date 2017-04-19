/* Socket.h
 * this file is part of Dynstack/RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */
#pragma once

#ifdef __linux__
#include <netinet/in.h>
#elif _WIN32
#include <winsock2.h>
#endif

#include <string>
#include <sstream>
#include <vector>

#include <atomic>
#include <mutex>


namespace lib
{
	namespace io
	{
		namespace network
		{
			/// TCP/IP Network Socket
			/**
			*   Basic platform independent socket implementation
			*/
			class Socket
			{
			private:

#ifdef __linux__
				int m_sock;
				static constexpr int waitFor_size = FD_SETSIZE;
#elif _WIN32
				SOCKET m_sock;
				static std::atomic<int> m_sock_counter;	/// Counts the number of running sockets to terminate WSA if no socket is running
				static std::mutex m_mutex;
#endif
				sockaddr_in m_addr;

			public:
				/// Basic constructor
				/**
				*   Initialise WSA on Win32 platforms
				*/
				Socket();

				/// Remove copy constructor
				/**
				*   Copy of connected socket creates problems
				*/
				Socket(const Socket& p_rhs) = delete;

				/// Move constructor
				Socket(Socket&& p_rhs);

				/// Destructor
				/**
				*   Close WSA if no socket exists anymore
				*/
				virtual ~Socket();

				/// Remove assignment operator
				/**
				*   Copy (assignment) of connected socket creates problems
				*/
				Socket& operator=(const Socket&) = delete;

				/// Socket initialization
				bool create();

				/// Close socket connection
				bool close();

				/// bind socket to port
				bool bind(const unsigned short p_port);

				/// listen to socket after bind
				bool listen() const;

				/// accept socket after listen
				/**
				*	\param[out] p_new_socket
				*/
				bool accept(Socket& p_new_socket) const;

				/// Client initialization and connect
				bool connect(const std::string p_host, const unsigned short p_port);

				/// Send driver
				bool send(const uint8_t* const p_data, const unsigned int p_length);

				/// Recieve message
				std::vector<uint8_t> recv(bool p_noBlock = false);					//Parameter (true) = unblocking

				/// Test if the socket connection is valid and no error is set
				bool is_valid() const;

				/// Get IP that is connected to this socket
				std::string getIP() const;

				/// Waits until a message to read is available or the timeout is hit
				static const std::vector<Socket*> waitFor_read(std::vector<Socket*> p_sock, const long long p_timeout_us);

				/// Waits until you can send a message or the timeout is hit
				static const std::vector<Socket*> waitFor_write(std::vector<Socket*> p_sock, const long long p_timeout_us);
			};

		}
	}
}
