 /* maincontrol_test.h
 * this file is part of Dynstack/RemoteControl for CORSIKA
 *
 * Copyright (C) <2018> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#pragma once

#include "zmq.hpp"

#include "remote_control/control/main_control.h"

#include <tuple>

namespace test
{
	namespace remote_control
	{

        auto router_recv(zmq::socket_t& s)
        {
            zmq::message_t idty(8);
            s.recv( &idty);

            zmq::message_t message(128);
            s.recv( &message);
            s.recv( &message);

            return std::make_tuple( std::move(idty), std::move(message));
        }

        template< class TAddress>
	    void router_send(zmq::socket_t& s, TAddress addr, void* msg, unsigned int size)
        {
            s.send(addr, sizeof(*addr), ZMQ_SNDMORE);
			s.send("", 0, ZMQ_SNDMORE);
			s.send(msg, size);
        }

        TEST(MAINCONTROL, Init)
        {
            zmq::context_t ctx(1);
            zmq::socket_t server(ctx, ZMQ_ROUTER);

            server.bind("tcp://*:55555");



            ASSERT_TRUE( ::remote_control::SMainControl().start("tcp://localhost:55555") );

            
            //auto [idty, msg]
            auto  data = router_recv(server);

            int* addr = reinterpret_cast<int*>( std::get<0>(data).data() );
            ASSERT_STREQ( reinterpret_cast<char*>(std::get<1>(data).data()), "startup");

            
            int status = 202;            
            router_send<int*>(server, addr, &status, sizeof(status));
         

            
            ::remote_control::SMainControl().stop();

            server.close();
            ctx.close();
        }





        TEST(MAINCONTROL, Send)
        {
            zmq::context_t ctx(1);
            zmq::socket_t server(ctx, ZMQ_ROUTER);

            server.bind("tcp://*:55554");

            ASSERT_TRUE(  ::remote_control::SMainControl().start("tcp://localhost:55554") );
            
            auto  data = router_recv(server);

            
            int* addr = reinterpret_cast<int*>( std::get<0>(data).data() );
            ASSERT_STREQ( reinterpret_cast<char*>(std::get<1>(data).data()), "startup");
           
            int status = 202;
            router_send<int*>(server, addr, &status, sizeof(status));
                               
            for( int i = 0; i<10; i++)
            {                

                ASSERT_LE( ::remote_control::SMainControl().send("Test", 5), 2) << "Message Queue should not fill up here";

                auto data = router_recv(server);

                ASSERT_STREQ( reinterpret_cast<char*>(std::get<1>(data).data()), "Test" );
                
                router_send(server, addr, &status, 4);
            }
            

            server.close();            
            ctx.close();

            ::remote_control::SMainControl().stop();
        }


        TEST(MAINCONTROL, Recv)
        {
            zmq::context_t ctx(1);
            zmq::socket_t server(ctx, ZMQ_ROUTER);

            server.bind("tcp://*:55554");

            ASSERT_TRUE(  ::remote_control::SMainControl().start("tcp://localhost:55554") );
            
            auto  data = router_recv(server);

            
            int* addr = reinterpret_cast<int*>( std::get<0>(data).data() );
            ASSERT_STREQ( reinterpret_cast<char*>(std::get<1>(data).data()), "startup");
           
            int status = 202;
            router_send<int*>(server, addr, &status, sizeof(status));
                               
            for( int i = 0; i<10; i++)
            {                

                ASSERT_LE( ::remote_control::SMainControl().send("Test", 5), 2) << "Message Queue should not fill up here";

                auto data = router_recv(server);
                ASSERT_STREQ( reinterpret_cast<char*>(std::get<1>(data).data()), "Test" );
                

                std::cout << "Status: " << 202 + i;

                status = 202 + i;
                router_send(server, addr, &status, 4);
                
                auto ret = ::remote_control::SMainControl().recv();
                ASSERT_EQ( 202 + i, *reinterpret_cast<int*>( ret.data() ) );

            }
            

            server.close();            
            ctx.close();

            ::remote_control::SMainControl().stop();
        }


    }
}