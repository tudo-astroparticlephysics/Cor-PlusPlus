/* main_control.cpp
 * this file is part of Dynstack/RemoteControl for CORSIKA
 *
 * Copyright (C) <2018> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#include "remote_control/control/main_control.h"

#include "zmq.hpp"

#include <iostream>
#include <string>
#include <chrono>

namespace remote_control
{

	

	void MainControl::loop()
	{
		zmq::context_t ctx(zmq::context_t(1));

		zmq::socket_t client(ctx, ZMQ_REQ);

		client.setsockopt(ZMQ_IDENTITY, 1234);		
		client.connect(m_address); // example: "tcp://localhost:5570")

		client.send("startup", 8);
		
		zmq::message_t message(20);
		client.recv(&message, 20);
				
		while(m_running)
		{
			if( m_msgBuffer.empty() )
			{
				std::this_thread::sleep_for( std::chrono::microseconds(1) );
			}
			else
			{				
				client.send( m_msgBuffer.front());
				m_msgBuffer.pop_front();

				
				zmq::message_t data(64);
				client.recv(&data);  

				m_recvBuffer.push_back( std::move(data) );              
			}
		}

		client.close();
		ctx.close();
	}

	MainControl::MainControl()
		: m_running(false)			
	{
		// 	context_t( Number of threads )       

        
	}

	MainControl::~MainControl()
	{
		

	}

	bool MainControl::start(const std::string address)
	{		
		m_address = address;

		m_running = true;

		m_loop = std::thread(&MainControl::loop, this);		

		return true;

	}

	void MainControl::stop()
	{		
		m_running = false;
		m_loop.join();				

		m_msgBuffer.clear();
		m_recvBuffer.clear();
	}

	int MainControl::send(const void* p, unsigned int len)
	{

		m_msgBuffer.emplace_back(p, len);

		return m_msgBuffer.size();
	}

	int MainControl::send(std::vector<char> p)
	{		
		m_msgBuffer.emplace_back(p.data(), p.size());

		return m_msgBuffer.size();
	}

	std::vector<char> MainControl::recv()
	{
		std::cout << "Length: " << m_recvBuffer.size();
		if( m_recvBuffer.empty() )
		{
			return std::vector<char>();
		}
		else
		{
			char* data = reinterpret_cast<char*>( m_recvBuffer.front().data() );

			std::vector<char> buf(data, data + m_recvBuffer.front().size() );

			m_recvBuffer.pop_front();

			std::cout << "Length: " << m_recvBuffer.size();

			return std::move(buf);
		}
	}

} /* namespace RemoteControl */
