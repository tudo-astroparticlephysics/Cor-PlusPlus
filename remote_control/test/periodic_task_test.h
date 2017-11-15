/* protocol_test.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once


#include "remote_control/control/periodic_task.h"

#include <chrono>
#include <thread>


namespace test
{

	namespace remote_control
	{

		TEST(PeriodicTask, Construction)
        {

			int counter = 0;

			::remote_control::PeriodicTask p1([&counter](){counter++;}, std::chrono::milliseconds(500) );

			ASSERT_EQ(counter, 0);
        }

		TEST(PeriodicTask, Call)
		{

			int counter = 0;
			::remote_control::PeriodicTask p1([&counter](){counter++;}, std::chrono::milliseconds(500) );

			ASSERT_EQ(counter, 0);
			p1.call();
			ASSERT_EQ(counter, 0);

			std::this_thread::sleep_for(  std::chrono::duration<double, std::milli>(200.0) );
			p1.call();
			std::this_thread::sleep_for(  std::chrono::duration<double, std::milli>(200.0) );
			p1.call();
			std::this_thread::sleep_for(  std::chrono::duration<double, std::milli>(200.0) );
			p1.call();
			ASSERT_EQ(counter, 1);


		}
	}


}
