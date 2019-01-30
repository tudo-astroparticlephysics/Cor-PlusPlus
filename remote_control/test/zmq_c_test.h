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

#include "zmq.h"

#include <string>
#include <thread>

namespace test
{
	namespace remote_control
	{

        TEST(ZMQ_C_COM, Send)
        {
            void *ctx = zmq_ctx_new ();
            void *client = zmq_socket (ctx, ZMQ_REQ);

            char identity[] = "Test";
            zmq_setsockopt(client, ZMQ_IDENTITY, identity, strlen(identity));

            
            zmq_connect(client, "tcp://localhost:5570");


            //zmq::pollitem_t items[] = {{&client, 0, ZMQ_POLLIN, 0}};

            EXPECT_NO_THROW({
                for (int i=0; i<2; i++) 
                {
                    zmq_send(client, "Message", 8, 0);
                }            
            });

            zmq_close(client);
            zmq_ctx_destroy(ctx);
        }

       

        TEST(ZMQ_C_COM, Start)
        {      
            void *ctx = zmq_ctx_new ();
            void *client = zmq_socket (ctx, ZMQ_REQ);
            void *server = zmq_socket (ctx, ZMQ_REP);

            zmq_bind(server, "tcp://*:5559");
            zmq_connect(client, "tcp://localhost:5559");
        
        
            for( int i = 0; i<10; i++)
            {
                char message[20];
                memset(message, '\0', 20);

                memcpy(message, "Test", 5);

                zmq_send(client,message, 5, 0);

                zmq_recv(server, message, 19, 0);
                zmq_send(server,message, 5, 0);

                zmq_recv(client, message, 19, 0);
            }
            
            zmq_close(client);
            zmq_close(server);
            
            zmq_ctx_destroy(ctx);
        }


        TEST(ZMQ_C_COM, Router)
        {  
            void *ctx = zmq_ctx_new ();
            void *server = zmq_socket (ctx, ZMQ_ROUTER);

            int tmp = 1;
            //zmq_setsockopt(server, ZMQ_ROUTER_MANDATORY, &tmp, sizeof(int));
            //zmq_setsockopt(server, ZMQ_IDENTITY, "ROUT", 4); 
            zmq_bind(server, "tcp://*:5560");
        

            auto worker = []()
            {
                void *ctx = zmq_ctx_new ();
                void *client = zmq_socket (ctx, ZMQ_REQ);
                
                zmq_setsockopt(client, ZMQ_IDENTITY, "1234", 4);
            
                zmq_connect(client, "tcp://localhost:5560");

                for( int i = 0; i<5; i++)
                {                    
                    char message[20];
                    memset(message, '\0', 20);

                    std::cout << "WORKER: Send"<< std::endl;   
                    zmq_send(client,"Message", 8, 0);
                    std::cout << "WORKER: Recv"<< std::endl;
                    zmq_recv(client, message, 19, 0);
                    std::cout << "WORKER: Loop End"<< std::endl;
                }
                zmq_close(client);
            };

            std::thread job(worker);

            for(int i=0; i<5; i++)
            {
                char message[20];
                char id[20];
                memset(message, '\0', 20);

                std::cout << "SERVER: Recv!" << std::endl;
                zmq_recv(server, id, 19, ZMQ_RCVMORE);
                zmq_recv(server, message, 19, ZMQ_RCVMORE);
                zmq_recv(server, message, 19, 0);
                std::cout << "SERVER: " << id << "  -  " << message << std::endl;

                zmq_send(server, "1234", 4, ZMQ_SNDMORE);
                zmq_send(server, "", 0, ZMQ_SNDMORE);
                zmq_send(server, message, 8, 0);

                std::cout << "SERVER: Send!" << std::endl;
            }           

            job.join();
            
            zmq_close(server);
            
            zmq_ctx_destroy(ctx);
        }
    }
}