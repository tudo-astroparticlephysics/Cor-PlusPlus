/* SocketTest.cpp
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/



#include "lib/io/network/socket.h"
#include "lib/algorithm/char_to_byte.h"

#include <thread>
#include <chrono>
#include <iostream>

#include <cstring>


namespace test
{
	namespace lib
	{
		namespace network
		{

		
		
			bool test_clientServerConnection()
			{
				::lib::io::network::Socket client;
				::lib::io::network::Socket server;

				std::cout << "Create Server: " << std::boolalpha << server.create() << std::endl;
				std::cout << "Bind: " << std::boolalpha << server.bind(13337) << std::endl;
				std::cout << "Listen: " << std::boolalpha << server.listen() << std::endl;


				std::cout << "Create Client: " << std::boolalpha << client.create() << std::endl;
				std::cout << "Connect: " << std::boolalpha << client.connect("127.0.0.1", 13337) << std::endl;

				std::this_thread::sleep_for(std::chrono::milliseconds(500));

				::lib::io::network::Socket acc;
				std::cout << "Accept: " << std::boolalpha << server.accept(acc) << std::endl;

				server.close();
				client.close();

				return true;
			}

			bool test_send()
			{
				auto fServer = [](){
					std::cout << "Start server" << std::endl;

					::lib::io::network::Socket server;

					std::cout << "Server create"<< std::endl;
					server.create();

					std::cout << "Server bind" << std::endl;
					server.bind(13338);

					std::cout << "Server listen" << std::endl;
					server.listen();

					::lib::io::network::Socket acc;
					server.accept(acc);

					std::cout << "Wait for client message:" << std::endl;
					auto a = acc.waitFor_read({ &acc }, 0);
					std::cout << "Get client message!" << std::endl;

					if (a.size() != 1)
					{
						std::cerr << "Wait does not work correctly!" << std::endl;
					}					

					auto msg = a[0]->recv(true);	

					if (msg.size() != 5)
					{
						std::cerr << "Error - Message size is not correct (Server)!" << std::endl;
						std::cerr << "Its: " << msg.size() << "  Should: 5" << std::endl;
					}

					if (memcmp(msg.data(), "Test\0", 5) != 0)
					{
						std::cerr << "Error - Message is not correct (Server)!" << std::endl;
						std::cerr << "Should: " << "Test\0" << "  Is: ";
						for (auto itr : msg)
							std::cerr << itr;
						std::cerr << std::endl;
					}

					acc.send(::lib::algorithm::char_to_byte("Test2\0").data(), 6);

				};

				std::thread thread(fServer);	
				
				std::this_thread::sleep_for(std::chrono::milliseconds(500));

				std::cout << "Start client" << std::endl;
				::lib::io::network::Socket client;
				client.create();				
				client.connect("127.0.0.1", 13338);
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				
				std::cout << "Send" << std::endl;
				client.send(::lib::algorithm::char_to_byte("Test\0").data(), 5);


				std::cout << "Recv" << std::endl;
				auto msg = client.recv(false);

				std::cout << "Check" << std::endl;
				if (msg.size() != 6)
				{
					std::cerr << "Error - Message size is not correct (Client)!" << std::endl;
					return false;
				}

				if (std::memcmp(msg.data(), "Test2\0", 6) != 0)
				{
					std::cerr << "Error - Message is not correct (Client)!" << std::endl;
					std::cerr << "Should: " << "Test\0" << "  Is: ";
					for (auto itr : msg)
						std::cerr << itr;
					std::cerr << std::endl;

					return false;
				}

				std::cout << "Join" << std::endl;
				thread.join();

				return true;
			}
			
		}

	}
}
