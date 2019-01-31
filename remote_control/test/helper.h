/* helper.h
 * this file is part of Cor++ for CORSIKA
 *
 * Copyright (C) <2019> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */
#pragma once

#include "zmq.hpp"

auto router_recv(zmq::socket_t& s)
{
    zmq::message_t idty(8);
    s.recv( &idty);

    zmq::message_t message(128);
    s.recv( &message);
    s.recv( &message);

    return std::make_tuple( std::move(idty), std::move(message));
}

template< class TAddress>
void router_send(zmq::socket_t& s, TAddress addr, void* msg, unsigned int size)
{
    s.send(addr, sizeof(*addr), ZMQ_SNDMORE);
    s.send("", 0, ZMQ_SNDMORE);
	s.send(msg, size);
}