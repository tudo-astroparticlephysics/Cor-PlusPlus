/* Packet.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/
#pragma once


#include <vector>
#include <memory>

namespace remote_control
{
	namespace  communication
	{
		/// Basic network data managment
		/**
		 *  Hostformat is in most cases little Endian
		 *  Network-Byte-Order is big Endian		
		 */		
		class Packet
		{
		private:

		protected:
			std::vector<uint8_t> m_data;

		public:
						
			Packet();
			Packet(uint32_t p_header, unsigned int p_size = 0);

			Packet(const Packet& p_rhs);
			Packet(Packet&& p_rhs);

			/// Create packet from raw data
			Packet(const uint8_t* p_data, const unsigned int p_len);
			Packet(const std::vector<uint8_t> p_data);

			const uint8_t* toByte();

			/// Checks the content with the against the length
			/**
			 * 	\ret Returns 0x00 if header and packet length matches header and length data
			 * 	\ret Returns 0x01 if length is not equal to length information stored inside
			 * 	\ret Returns 0x02 if length is not equal to length interval defined by packet type
			 */
			int check() const;
			



			template<class T>
			void append(const T p_data);

			template<class T> 
			void append(std::unique_ptr<T> p_data);

			template<class T>
			void append(std::unique_ptr<T[]> p_data, const int p_size);

			template<class T>
			void append(const T* const p_data, const int p_size);


			inline unsigned int size() const{return static_cast<unsigned int>( m_data.size() );}

			inline uint32_t header() const { return *reinterpret_cast<const uint32_t*>(m_data.data() + 4); }

			inline bool empty() const { return m_data.empty();}

			inline const std::vector<uint8_t> data() const
			{
				if(m_data.size() < 8)
					return std::vector<uint8_t>();
				else
					return std::vector<uint8_t>(m_data.begin()+8, m_data.end());
			}

		};


		template<class T>
		void Packet::append(const T p_data)
		{
			m_data.reserve(m_data.size() + sizeof(T));

			for (unsigned int i = 0; i < sizeof(T); i++)
				m_data.push_back(reinterpret_cast<const uint8_t* const>(&p_data)[i]);
		}		

		template<class T>
		void Packet::append(std::unique_ptr<T> p_data)
		{
			m_data.reserve(m_data.size() + sizeof(T));

			for (int i = 0; i < sizeof(T); i++)
				m_data.push_back(reinterpret_cast<uint8_t*>(p_data.get())[i]);

		}

		template<class T>
		void Packet::append(std::unique_ptr<T[]> p_data, const int p_size)
		{
			m_data.reserve(m_data.size() + p_size);
			m_data.insert(m_data.end(), &p_data[0], &p_data[p_size]);
		}

		template<class T>
		void Packet::append(const T* const p_data, const int p_size)
		{
			m_data.reserve(m_data.size() + p_size);

			for(int i=0; i< p_size; i++)
				m_data.push_back(reinterpret_cast<const uint8_t* const>(p_data)[i]);
		}


	}

}









