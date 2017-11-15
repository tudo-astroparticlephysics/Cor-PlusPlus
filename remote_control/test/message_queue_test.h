/* message_queue_test.h
 * this file is part of Dynstack/RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#pragma once

#include "remote_control/communication/message_queue.h"
#include "remote_control/communication/packet_id.h"

#include <chrono>
#include <thread>

namespace test
{
	namespace remote_control
	{

        TEST(MessageQueueTest, SingleInsert)
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

                ASSERT_EQ(reinterpret_cast<const int*>(ret)[0], 12);
                ASSERT_EQ(reinterpret_cast<const int*>(ret)[1], i);
                ASSERT_EQ(reinterpret_cast<const int*>(ret)[2], i+3);
			}

        }


        TEST(MessageQueueTest, ThreadedInsert)
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

                ASSERT_EQ(reinterpret_cast<const int*>(ret)[0], 12);
                ASSERT_EQ(reinterpret_cast<const int*>(ret)[1], i);
                ASSERT_EQ(reinterpret_cast<const int*>(ret)[2], i+3);

				std::chrono::duration<double> diff = time2 - time1;
				if (i != 5)
				{
                    ASSERT_GT(diff.count(), 0.005);
                    ASSERT_LT(diff.count(), 0.020);
				}
				else
				{
                    ASSERT_GT(diff.count(), 0.95);
                    ASSERT_LT(diff.count(), 1.05);
				}
            }
            t.join();
        }


	}
}
