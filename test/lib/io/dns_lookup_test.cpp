/* dns_lookup_test.cpp
 * this file is part of Dynstack/RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#include "lib/io/network/dns_lookup.h"

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
			void ping(std::string address)
			{
				int sys_return = 0;
#ifdef __linux__
				sys_return = system( (std::string("ping ") + address + std::string(" -c 1")).c_str() );
#elif _WIN32
				sys_return = system( (std::string("ping ") + address + std::string(" -n 1")).c_str() );
#endif
				(void)(sys_return);
			}

			bool test_dnsLookup()
			{
				std::cout << "google.de >> " << ::lib::io::network::hostname_to_ip("google.de") << std::endl;
				ping("google.de");
				std::this_thread::sleep_for(std::chrono::seconds(1));


				std::cout << "vollmond.app.tu-dortmund.de >> " << ::lib::io::network::hostname_to_ip("vollmond.app.tu-dortmund.de") << std::endl;
				ping("vollmond.app.tu-dortmund.de");
				std::this_thread::sleep_for(std::chrono::seconds(1));
				
				std::cout << "129.217.160.98 >> " << ::lib::io::network::hostname_to_ip("129.217.160.98") << std::endl;
				ping("129.217.160.98");
				std::this_thread::sleep_for(std::chrono::seconds(1));

				try
				{
					std::cout << "abc >> " << ::lib::io::network::hostname_to_ip("abc") << std::endl;
					ping("abc");
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}
				catch(std::exception e)
				{
					std::cout << "Exception: "<< e.what() << std::endl;
				}

				

				return true;
			}

		}

	}
}
