/* maincontrol_test.h
 * this file is part of Dynstack/RemoteControl for CORSIKA
 *
 * Copyright (C) <2019> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */
#pragma once

#include "zmq.hpp"

#include "remote_control/export.h"
#include "helper.h"

#include <array>
#include <algorithm>
#include <numeric>

namespace test
{
	namespace remote_control
	{       

        std::thread TestServer;
        std::atomic<bool> testServer_Running;

        TEST(EXPORT, start_localServer)
        {
            testServer_Running = true;

            auto lambda = [](){
                zmq::context_t ctx(1);
                zmq::socket_t server(ctx, ZMQ_ROUTER);

                server.bind("tcp://*:55554");

                while(testServer_Running)
                {
                    std::cout << "Loop!" << std::endl;

                    //auto [idty, msg]
                    auto  data = router_recv(server);

                    int* addr = reinterpret_cast<int*>( std::get<0>(data).data() );
                    reinterpret_cast<char*>(std::get<1>(data).data());

                    std::cout << "Send:" << std::endl;
                        
                    router_send<int*>(server, addr, std::get<1>(data).data(), std::get<1>(data).size() );
                }
            };

            TestServer = std::thread(lambda);

            std::this_thread::sleep_for( std::chrono::seconds(1) );
        }

        TEST(EXPORT, setup)
        {
            std::string str("RC_IP tcp://127.0.0.1:55554");
            int len = str.length();

            ASSERT_NO_THROW({
                remotecontrol_init_(str.c_str(), &len);
            });
            std::this_thread::sleep_for( std::chrono::seconds(1) );
        }

        TEST(EXPORT, start)
        {        
            ASSERT_NO_THROW({
                remotecontrol_start_();
            });
            std::this_thread::sleep_for( std::chrono::seconds(1) );
        }

        TEST(EXPORT, evth)
        {
            std::array<float, 273> arr;
            std::iota(arr.begin(), arr.end(), 273*0);

            ASSERT_NO_THROW( remotecontrol_push_evth_( arr.data() ) );
            std::this_thread::sleep_for( std::chrono::seconds(1) );
        }

        TEST(EXPORT, evte)
        {

            std::array<float, 273> arr;
            std::iota(arr.begin(), arr.end(), 273*1);

            ASSERT_NO_THROW( remotecontrol_push_evte_( arr.data() ) );
            std::this_thread::sleep_for( std::chrono::seconds(1) );
        }

        TEST(EXPORT, runh)
        {

            std::array<float, 273> arr;
            std::iota(arr.begin(), arr.end(), 273*2);

            ASSERT_NO_THROW( remotecontrol_push_runh_( arr.data() ) );
            std::this_thread::sleep_for( std::chrono::seconds(1) );
        }

        TEST(EXPORT, end)
        {
            testServer_Running = false;

            std::array<float, 273> arr;
            std::iota(arr.begin(), arr.end(), 273*3);
            ASSERT_NO_THROW( remotecontrol_push_rune_( arr.data() ) );

            std::this_thread::sleep_for( std::chrono::seconds(1) );

            ASSERT_NO_THROW({
                remotecontrol_end_();
            });
            std::this_thread::sleep_for( std::chrono::seconds(1) );
        }

        TEST(EXPORT, stop_localServer)
        {
            TestServer.join();

            std::this_thread::sleep_for( std::chrono::seconds(1) );
        }
    }
}