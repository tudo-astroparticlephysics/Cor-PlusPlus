/* client_test.cpp
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/



#include "lib/io/network/client.h"
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

			bool test_client()
			{
											

				std::thread tr([]() -> bool {
					::lib::io::network::Socket server;
					server.create();
					server.bind(9557);
					server.listen();

					::lib::io::network::Socket con;
					server.accept(con);
					
					auto data = con.recv();
					if (std::memcmp(data.data(), "Test\0", 5) != 0)
					{
						std::cerr << "Client socket sends wrong data!\a" << std::endl;
						return false;
					}
					con.close();
					server.close();

					return true;
				});
				
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				::lib::io::network::Client client;
				client.init("127.0.0.1", 9557);

				client.send( ::lib::algorithm::char_to_byte("Test\0").data(), 5);

				client.close();

				std::cout << "Wait for join..." << std::endl;
				tr.join();

				return true;
			}
			
		}

	}
}
