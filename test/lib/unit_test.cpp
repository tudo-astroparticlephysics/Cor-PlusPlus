/* lib.cpp
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#include <iostream>
#include <string>


#include "test/lib/data/particle_test.h"

#include "test/lib/io/socket_test.h"
#include "test/lib/io/dns_lookup_test.h"
#include "test/lib/io/client_test.h"
#include "test/lib/io/server_test.h"

#include "test/lib/meta/list_test.h"
#include "test/lib/meta/tuple_test.h"
#include "test/lib/meta/is_callable_test.h"


namespace test
{
	namespace lib
	{
		bool test_data()
		{
			bool erg = true;

			std::cout << "\n\t-Test Data-" << std::endl;
			std::cout << "\nparticele data:" << std::endl;
			erg = erg && data::test_particle();

			return erg;
		}


		bool test_io()
		{
			bool erg = true;

			std::cout << "\n\t-Test Socket-" << std::endl;
			std::cout << "\nclientServerConnection:" << std::endl;
			erg = erg && network::test_clientServerConnection();
			std::cout << "\nsend" << std::endl;
			erg = erg && network::test_send();

			std::cout << "\n\t-Test Client-" << std::endl;
			erg = erg && network::test_client();

			std::cout << "\n\t-Test Server-" << std::endl;
			erg = erg && network::test_server();

			std::cout << "\n\t-Test DNS Lookup-" << std::endl;
			erg = erg && network::test_dnsLookup();

			return erg;
		}

		bool test_meta()
		{
			bool erg = true;

			std::cout << "\n\t-Test tuple-" << std::endl;
			std::cout << "\ntupleFilter:" << std::endl;
			erg = erg && meta::test_tupleFilter();
			std::cout << "\ntupleFold:" << std::endl;
			erg = erg && meta::test_tupleFold();
			std::cout << "\ntupleMap" << std::endl;
			erg = erg && meta::test_tupleMap();

			std::cout << "\n\t-Test list-" << std::endl;
			erg = erg && meta::test_compileTimeList();

			std::cout << "\n\t-Test is_callable-" << std::endl;
			erg = erg && meta::test_isCallable();

			return erg;
		}
	}

	bool test_lib()
	{
		std::cout << std::string(8, '\n');
		std::cout << "\t\t\t--- Basic ---" << std::endl;

		bool erg = true;

		std::cout << "\n\n\t\t-- Data --\n" << std::endl;
		erg = erg && lib::test_data();

		std::cout << "\n\n\t\t-- IO --\n" << std::endl;
		erg = erg && lib::test_io();

		std::cout << "\n\n\t\t-- Meta --\n" << std::endl;
		erg = erg && lib::test_meta();

		return erg;
	}

	
}



int main()
{
        std::cout << "Start testing ... \n" << std::endl;

        test::test_lib();     


        std::cout << "\n\n\tAll tests finished" << std::endl;

        return 0;

}

