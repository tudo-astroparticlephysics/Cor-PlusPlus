 /* zmq_test.h
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

#include <string>
#include <thread>

namespace test
{
	namespace remote_control
	{

        TEST(ZMQ_CPP_COM, Send)
        {
            zmq::context_t ctx(1);
            zmq::socket_t client(ctx, ZMQ_REQ);

            char identity[] = "Test";

            client.setsockopt(ZMQ_IDENTITY, identity);



            client.connect("tcp://localhost:5570");
            

            //zmq::pollitem_t items[] = {{&client, 0, ZMQ_POLLIN, 0}};

            // 
            EXPECT_NO_THROW({ //ASSERT_NO_THROW
                for (int i=0; i<2; i++) 
                {
                    //client.send("Message", 8);

                    //zmq::message_t message(20);
                    //client.recv(&message, ZMQ_NOBLOCK);
                }            
            }) << "Fails when no seperate server is available! ";

        }

       

        TEST(ZMQ_CPP_COM, Start)
        {      
            zmq::context_t ctx(1);
            zmq::socket_t client(ctx, ZMQ_REQ);
            zmq::socket_t server(ctx, ZMQ_REP);


            client.connect("tcp://localhost:5559");
            server.bind("tcp://*:5559");
        
            for( int i = 0; i<10; i++)
            {
                zmq::message_t message(20);

                std::memcpy(message.data(), "Test", 5);

                client.send(message);

                server.recv(&message);
                server.send(message);

                client.recv(&message);
            }
            
            client.close();
            server.close();
            
            ctx.close();
        }


        TEST(ZMQ_CPP_COM, Router)
        {  
            zmq::context_t ctx(1);	
            zmq::socket_t server(ctx, ZMQ_ROUTER);

            
            server.bind("tcp://*:5559");
        

            auto worker = []()
            {
                zmq::context_t ctx(1);
            
                zmq::socket_t client(ctx, ZMQ_REQ);
                client.setsockopt(ZMQ_IDENTITY, 1234);
                client.connect("tcp://localhost:5559");	

                for( int i = 0; i<5; i++)
                {                    
                    zmq::message_t message(50);
                    std::memcpy(message.data(), "Test", 5);

                    //std::cout << "WORKER: Send"<< std::endl;
                    client.send(message);
                    //std::cout << "WORKER: Recv"<< std::endl;
                    client.recv(&message);
                    //std::cout << "WORKER: Loop End"<< std::endl;
                }
                client.close();
            };

            std::thread job(worker);

            zmq::message_t id(20);
            zmq::message_t message(20);
            for(int i=0; i<5; i++)
            {
                //std::cout << "SERVER: Recv!" << std::endl;
                server.recv(&id);
                server.recv(&message);
                server.recv(&message);
                //std::cout << "SERVER: " << *(int*)id.data() << "  -  " << (const char*)message.data() << std::endl;

                server.send(id, ZMQ_SNDMORE);
                server.send("", 0, ZMQ_SNDMORE);
                server.send(message);

                //std::cout << "SERVER: Send!" << std::endl;
            }
            
            job.join();

            server.close();
            
            ctx.close();
        }
    }
}