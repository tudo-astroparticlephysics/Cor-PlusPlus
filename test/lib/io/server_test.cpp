/* server_test.cpp
 * this file is part of Dynstack/RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#include "lib/io/network/server.h"
#include "lib/io/network/socket.h"

#include "lib/algorithm/char_to_byte.h"

#include <thread>
#include <chrono>
#include <iostream>

#include <cstring>
#include <cstdlib>

namespace test
{
	namespace lib
	{
		namespace network
		{

			bool test_server()
			{

				const int port = 5000 + (rand() % 5000);				
				::lib::io::network::Server server(port);
				server.setNewConFunc([](unsigned int id, ::lib::io::network::Socket* sock)->bool
				{					
					(void)(id);
					(void)(sock);
					return true;
				});

				server.setRecvFunc([](unsigned int id, std::vector<uint8_t> data)->void
				{
					(void)(id);
					if (std::memcmp(data.data(), "T3st\0", 5) != 0)
					{
						std::cerr << "Client socket sends wrong data!\a" << std::endl;
					}
				});


				std::this_thread::sleep_for(std::chrono::seconds(1));
				


				std::thread tr([port]() -> bool
				{
					::lib::io::network::Socket client;
					client.create();
					client.connect("127.0.0.1", port);

					client.send( ::lib::algorithm::char_to_byte("T3st\0").data(), 5);
					
					auto data = client.recv();
					if (std::memcmp(data.data(), "Test\0", 5) != 0)
					{
						std::cerr << "Server socket sends wrong data!\a" << std::endl;						
					}
					
					client.close();

					
					return true;
				});
								
				server.broadcast(::lib::algorithm::char_to_byte("Test\0").data(), 5);

				tr.join();
				
				return true;
			}

		}

	}
}
