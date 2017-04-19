/* client.cpp
 * this file is part of Dynstack/RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#include "lib/io/network/client.h"

#include <exception>

#include "lib/io/io_exception.h"

namespace lib
{
	namespace io
	{
		namespace network
		{
			Client::Client()
			{

			}

			Client::~Client()
			{

			}

			bool Client::init(const std::string p_ip, const unsigned short p_port)
			{
				if (!this->m_oConnection.create())
				{
					std::cerr << "Client could create socket (" << p_ip << ":" << p_port << ")" << std::endl;
					throw(lib::io::network::socket_creation_failed());
				}


				return this->m_oConnection.connect(p_ip, p_port);
			}

			void Client::close()
			{
				this->m_oConnection.close();
			}

			bool Client::send(const uint8_t* const p_data, const unsigned int p_len)
			{
				return this->m_oConnection.send(p_data, p_len);
			}
			
			bool Client::send(std::vector<uint8_t> p_data)
			{
				return this->m_oConnection.send(p_data.data(), static_cast<const unsigned int>(p_data.size()) );
			}

			std::vector<uint8_t> Client::recv()
			{
				return this->m_oConnection.recv();
			}

			std::vector<uint8_t> Client::recv(std::chrono::microseconds p_timeout)
			{
				auto con = this->m_oConnection.waitFor_read( { &m_oConnection }, p_timeout.count());

				if (!con.empty())
					return this->m_oConnection.recv();

				return std::vector<uint8_t>();
			}

		}
	}
}
