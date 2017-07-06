/* unit_test.cpp
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#include "test/remote_control/protocol_test.h"

#include "test/remote_control/message_queue_test.h"

namespace test
{
	namespace remote_control
	{
		bool test_mesageQueue()
		{
			bool erg = true;

			std::cout << "\n\t-Single Insert-" << std::endl;
			erg = erg && test::remote_control::test_mq_singleInsert();

			std::cout << "\n\t-Threaded Insert-" << std::endl;
			erg = erg && test::remote_control::test_mq_threadedInsert();

			return erg;
		}

		bool test_protocol()
		{
			bool erg = true;

			std::cout << "\n\t-Protocol construction-" << std::endl;
			erg = erg && test::remote_control::test_protocol_construction();

			return erg;
		}

	}

	bool test_remoteControl()
	{
		std::cout << std::string(8, '\n');
		std::cout << "\t\t\t--- RemoteControl Test ---" << std::endl;

		bool erg = true;	

		std::cout << "\n\n\t\t--Test protocol--" << std::endl;
		erg = erg && test::remote_control::test_protocol();

		std::cout << "\n\n\t\t--Test message queue--" << std::endl;
		erg = erg && test::remote_control::test_mesageQueue();

		return erg;
	}
}



int main()
{
        std::cout << "Start testing ... \n" << std::endl;

        test::test_remoteControl();





        std::cout << "\n\n\t All tests finished" << std::endl;

        return 0;

}


