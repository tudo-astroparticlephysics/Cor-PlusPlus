/* main_control.cpp
 * this file is part of Dynstack/RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#include "remote_control/control/main_control.h"

#include "lib/io/network/dns_lookup.h"

namespace remote_control
{

	void MainControl::loop()
	{
		std::cout << "(RC) Start main loop..." << std::endl;
		//auto start_time = std::chrono::high_resolution_clock::now();

		try
		{
			while (m_tsRunning == true)
			{
				const auto msg_recv = m_client.recv(std::chrono::milliseconds(5));

				if (!msg_recv.empty())
				{
					//evaluate message
					remote_control::communication::Packet recv_packet(msg_recv);

					// Check for packet consistency, returns 0 if everything is fine else an error code
					// C++17 allows if with initializer
					int check = recv_packet.check();
					if( check )
					{
						std::cerr << "Received a compromised package!" << std::endl;
					}
					else
					{
						// Send to callback functions
						auto callback = m_callback.find( recv_packet.header() );
						if(callback != m_callback.end())
						{
							callback->second( recv_packet.data() );
						}
					}

				}

				if (!m_tsQueue.empty())
				{
					auto msg_send = m_tsQueue.pop_front_blocking();

					if (!m_client.send(msg_send.toByte(), static_cast<const unsigned int>(msg_send.size()) ))
					{
						std::cerr << __FILE__ << " Message could not be send" << std::endl;
					}
				}

				/// Call periodic functions
				auto current_time = std::chrono::high_resolution_clock::now();

				for(auto itr : m_periodic)
				{
					itr.call( current_time );
				}

			}
		} catch (std::exception e)
		{
			std::cerr << __FILE__ << " Exception in main loop: " << e.what() << std::endl;
		}

		/// Terminate all connections
		std::cout << "(RC) Close main loop ..." << std::endl;
		m_client.close();

		std::cout << "(RC) Main loop closed" << std::endl;
	}

	MainControl::MainControl()
			: m_periodic(register_periodic_callback()), m_callback(register_server_callback())
	{
		m_tsRunning = false;
	}

	MainControl::~MainControl()
	{

	}

	bool MainControl::start(const std::string dns, const unsigned short port)
	{
		if (m_tsRunning == true)
		{
			return false;
		}

		const std::string ip = lib::io::network::hostname_to_ip(dns);
		if (!m_client.init(ip, port))
		{
			std::cerr << "Could not connect to " << dns << ":" << port << "(" << ip << ")" << std::endl;
			return false;
		}

		m_tsRunning = true;
		m_thread = std::thread(&MainControl::loop, this);

		return true;

	}

	void MainControl::stopp()
	{
		if (m_tsRunning == true && m_thread.joinable())
		{
			m_tsRunning = false;
			m_thread.join();
		}
	}

	void MainControl::send(remote_control::communication::Packet p)
	{
		this->m_tsQueue.push_back(p);
	}

} /* namespace RemoteControl */
