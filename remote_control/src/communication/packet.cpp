/* packet.cpp
 * this file is part of Dynstack/RemoteControl for CORSIKA
 *
 * Copyright (C) <2019> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#include "remote_control/communication/packet.h"

#include <cstring>



namespace remote_control
{
	namespace  communication
	{
        //Packet::Packet(int id)
       


        Packet::Packet(Packet&& rhs)
        {
            this->m_data = std::vector<char>( std::move(rhs.m_data) );
        }
    }
}
