/* packet.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2019> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#include <vector>
#include <cstring>

#pragma once


namespace remote_control
{
	namespace communication
	{

        class Packet
        {
            private:
                
                std::vector<char> m_data;

            public:

                Packet(int id)
                {
                    this->m_data.resize(8);
                    memcpy(&id, this->m_data.data(), sizeof(int) );
                }

                Packet(Packet&& rhs);

                template<class TType>
                Packet(int id, TType data)
                    : Packet(id)
                {
                    this->setData<TType>(data);
                }

                template<class TType>
                Packet(int id, TType* data, int len)
                    : Packet(id)
                {
                    this->setData<TType>(data, len);
                }


                template<class TType>
                void addData(TType data)
                {
                    int oldSize = m_data.size();

                    m_data.resize( oldSize + sizeof(data) );
                    std::memcpy(this->m_data.data() + oldSize, &data, sizeof(TType) );
                }

                template<class TType>
                void addData(const TType* data, int len)
                {
                    int oldSize = m_data.size();

                    m_data.resize( oldSize + sizeof(data)*len );
                    std::memcpy(this->m_data.data() + oldSize, data, sizeof(TType)*len );
                }

               
                template<class TType>
                void setData(TType data)
                {
                    int packetId = *reinterpret_cast<int*>(this->m_data.data());

                    m_data.resize(sizeof(int) + sizeof(TType));
                    
                    std::memcpy(this->m_data.data(), &packetId, sizeof(int) );
                    std::memcpy(this->m_data.data() + sizeof(int), &data, sizeof(TType) );
                }

                template<class TType>
                void setData(const TType* data, int len)
                {
                    const int packetId = *reinterpret_cast<int*>(this->m_data.data());

                    m_data.resize(sizeof(int) + sizeof(TType)*len);
                    
                    std::memcpy(this->m_data.data(), &packetId, sizeof(int) );
                    std::memcpy(this->m_data.data() + sizeof(int), data, sizeof(TType)*len );
                }

                std::vector<char> getData() const {return m_data;}

        };

    }
}