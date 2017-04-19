/* client.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once

#include "lib/io/network/socket.h"

#include <chrono>

namespace lib
{
	namespace io
	{
		namespace network
		{



			/// TCP Client
			/**
			* 	Easy interface to create an client with the the \ref Socket class
			*/
			class Client
			{
			private:

				/// Platform independet socket connection
				Socket m_oConnection;

				//std::function<void(Client, unsigned int, std::vector<char>&&)> m_std_callback;

			public:

				/// Client constructor
				Client();

				/// Client destructor
				~Client();

				/// Client initialisation
				/**
				*   \param[in] p_ip ip or dns of the target host
				*   \param[in] p_port port of the target host (1 - 65535)
				*/
				bool init(const std::string p_ip, const unsigned short p_port);

				/// Close connection
				void close();


				/// Send byte data
				/**
				*   \param[in] p_data Array of unsigned byte data
				*   \param[in] p_len length of the array
				*/
				bool send(const uint8_t* const p_data, const unsigned int p_len);

				/// Send byte data
				/**
				*   \param[in] p_data Vector of unsigned byte data
				*/
				bool send(std::vector<uint8_t> p_data);
				
				/// Recv byte data
				/**
				*	Blocking call, waits until data is recieved
				*   \return Vector of unsigned byte data
				*/
				std::vector<uint8_t> recv();

				/// Recv byte data
				/**
				*   Blocks for the given amount of time, returns the data that got recieved until timeout occours
				*   \param[in] p_timeout time in microseconds
				*	\return Vector of recieved unsigned byte data 
				*/
				std::vector<uint8_t> recv(std::chrono::microseconds p_timeout);



			};

		}
	}
}
