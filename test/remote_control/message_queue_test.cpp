/* message_queue_test.cpp
 * this file is part of Dynstack/RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#include "test/remote_control/message_queue_test.h"

#include "remote_control/communication/message_queue.h"
#include "remote_control/communication/packet_id.h"

#include <iostream>
#include <string>

#include <chrono>
#include <thread>

namespace test
{
	namespace remote_control
	{
		bool test_mq_singleInsert()
		{
			::remote_control::communication::MessageQueue queue;

			for (int i = 0; i < 10; i++)
			{
				::remote_control::communication::MessageQueue::storage_type data(i);

				data.append(i+3);

				queue.push_back(data);
			}

			for (int i = 0; i < 10; i++)
			{

				auto tmp = queue.pop_front_blocking();

				auto ret = tmp.toByte();

				if (reinterpret_cast<const int*>(ret)[0] != 12)
				{
					std::cerr << "Wrong size in queue: " << reinterpret_cast<const int*>(ret)[0] << "/" << 12 << std::endl;
					return false;
				}
				if (reinterpret_cast<const int*>(ret)[1] != i)
				{
					std::cerr << "Wrong header in queue: " << reinterpret_cast<const int*>(ret)[1] << "/" << i << std::endl;
					return false;
				}
				if (reinterpret_cast<const int*>(ret)[2] != i+3)
				{
					std::cerr << "Wrong data in queue: " << reinterpret_cast<const int*>(ret)[2] << "/" << i+3 << std::endl;
					return false;
				}
			}

			return true;

		}

		bool test_mq_threadedInsert()
		{
			::remote_control::communication::MessageQueue queue;


			auto f = [&queue]() {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				for (int i = 0; i < 5; i++)
				{
					::remote_control::communication::MessageQueue::storage_type data(i);

					data.append(i + 3);

					queue.push_back(data);
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				for (int i = 5; i < 10; i++)
				{
					::remote_control::communication::MessageQueue::storage_type data(i);

					data.append(i + 3);

					queue.push_back(data);
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
			};

			std::thread t(f);

			for (int i = 0; i < 10; i++)
			{
				auto time1 = std::chrono::high_resolution_clock::now();

				auto tmp = queue.pop_front_blocking();

				auto time2 = std::chrono::high_resolution_clock::now();

				auto ret = tmp.toByte();

				if (reinterpret_cast<const int*>(ret)[0] != 12)
				{
					std::cerr << "Wrong size in queue: " << reinterpret_cast<const int*>(ret)[0] << "/" << 12 << std::endl;
					return false;
				}
				if (reinterpret_cast<const int*>(ret)[1] != i)
				{
					std::cerr << "Wrong header in queue: " << reinterpret_cast<const int*>(ret)[1] << "/" << i << std::endl;
					return false;
				}
				if (reinterpret_cast<const int*>(ret)[2] != i + 3)
				{
					std::cerr << "Wrong data in queue: " << reinterpret_cast<const int*>(ret)[2] << "/" << i + 3 << std::endl;
					return false;
				}

				std::chrono::duration<double> diff = time2 - time1;
				if (i != 5)
				{
					if (diff.count() < 0.005 || diff.count() > 0.015)
					{
						std::cerr << "Time between pops wrong: " << i << " | " << diff.count() << "/" << 0.010 << std::endl;
						t.join();
						return false;
					}
				}
				else
				{
					if (diff.count() < 0.950 || diff.count() > 1.050)
					{
						std::cerr << "Time between pops wrong: " << i << " | " << diff.count() << "/" << 1.000 << std::endl;
						t.join();
						return false;
					}
				}			

				
			}

			t.join();
			return true;
		}

	}
}
