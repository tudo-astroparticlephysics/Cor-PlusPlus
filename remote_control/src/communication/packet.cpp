/* packet.cpp
 * this file is part of Dynstack/RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#include "remote_control/communication/packet.h"



#include <iostream>


namespace remote_control
{
	namespace communication
	{

		Packet::Packet()
		{
			m_data.reserve(0);
		}


		Packet::Packet(uint32_t p_header, unsigned int p_size)
		{
			m_data.reserve(8);

			const int32_t l = 8;

			// push_back 4 byte for packet length
			for (int i = 0; i < 4; i++)
				m_data.push_back( reinterpret_cast<const uint8_t*>(&l)[i] );

			// push_back 4 byte for packet header
			for (int i = 0; i < 4; i++)
				m_data.push_back( reinterpret_cast<const uint8_t*>(&p_header)[i] );



			m_data.reserve(p_size + 8);
		}

		Packet::Packet(const Packet& p_rhs)
		{
			this->m_data = p_rhs.m_data;
		}

		Packet::Packet(Packet&& p_rhs)
		{
			this->m_data = std::move(p_rhs.m_data);
		}

		Packet::Packet(const uint8_t* p_data, const unsigned int p_len)
		{
			m_data.reserve(p_len);

			for(unsigned int i=0; i<p_len; i++)
			{
				m_data.push_back( p_data[i] );
			}
		}

		Packet::Packet(const std::vector<uint8_t> p_data)
		{
			m_data = p_data;
		}



		const uint8_t* Packet::toByte()
		{
			if( this->empty() )
				return nullptr;


			int32_t l = static_cast<int32_t>(m_data.size());

			m_data[0] = reinterpret_cast<uint8_t*>(&l)[0];
			m_data[1] = reinterpret_cast<uint8_t*>(&l)[1];
			m_data[2] = reinterpret_cast<uint8_t*>(&l)[2];
			m_data[3] = reinterpret_cast<uint8_t*>(&l)[3];

			return m_data.data();
		}

		int Packet::check() const
		{
			const int32_t* data = reinterpret_cast<const int32_t*>( m_data.data() );

			const int32_t length = data[0];
			const int32_t header = data[1];

			if(length != static_cast<int>(m_data.size()) )
			{
				std::cerr << " -> remote_control Packet: Length in data is not equal to the length of the packet. Some data got compromised!";
				return 0x01;
			}
			if(header > 0x00)
			{
				std::cerr << " -> remote_control Packet: header defines different package length. Some data got compromised!";
				return 0x02;
			}

			return 0x00;
		}

	}

}

