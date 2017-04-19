/* server.h
 * this file is part of Dynstack/RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#pragma once

#include "lib/io/network/socket.h"

#include <functional>
#include <thread>
#include <atomic>
#include <mutex>

#include <map>

#include <iostream>

namespace lib
{
	namespace io
	{

		namespace network
		{
			/// TCP Server
			/**
			*   Basic threaded tcp server that allowes the registration of multiple callback methods
			*/
			class Server
			{
				/// typedef for a map of sockets (id to socket) 
				typedef std::map<unsigned int, std::unique_ptr<lib::io::network::Socket> > connectionHandler;

				/// typedef for callback function type
				typedef std::function<void(connectionHandler* const)> action;

			private:

				/// Basic listen socket
				/**
				*   Most basic socket that opens the basic server port and handles all incomming connection
				*/
				Socket m_arb_listenSock;

				/// ID counter
				/**
				*   Atomic counter that gets incremented with every incomming connection. It is used to set give every socket an unique id.
				*/
				std::atomic<unsigned int> m_nextID;
				
				/// Map of accepted connections
				connectionHandler m_std_connectionSock;

				/// New message callback
				/**
				*   first parameter is the socket id
				*   second parameter is the message
				*/
				std::function<void(unsigned int, std::vector<uint8_t>&&)> m_func_recv;

				/// New connection callback
				/**
				*    first parameter is the socket id
				*    second parameter new socket
				*    return true -> socket gets saved else discarded
				*/
				std::function<bool(unsigned int, Socket*)> m_func_newConnection;		

				/// Action storage
				/**
				*    Stores all specific ations (send, broadcast, disconnect) in form of a lambda function.
				*    The functions get executed in the main loop after a recv from all sockets
				*/
				std::vector< action > m_action;


				/// Thread that handles connection atemps
				std::thread* m_threadNewCon;

				/// Thread that handles all send/recieve tasks
				std::thread* m_threadNewData;

				/// termination condition for booth threads
				std::atomic<bool> m_isRunning;

				/// Lock mutex for action storage
				std::mutex m_actionMutex;

				/// Lock mutex for the socket vector
				std::mutex m_vectorMutex;

				/// Lock mutex for recv function
				std::mutex m_functionMutex;

				void handleConnection();
				void handleRecv();

			public:

				/// Creates server
				/**
				*   Creates the server and start all threads
				*   \param p_port Listening port of the server
				*/
				Server(const unsigned short p_port);

				/// Destructor
				/**
				*   Stops all running threads and waits for there termination				
				*/
				~Server();

				/// Send data
				/**
				*   \param[in] p_id ID of the connection to send the data
				*   \param[in] p_data unsigned byte array of the data that must be send
				*   \param[in] p_len length of the data array
				*/
				void send(unsigned int p_id, const uint8_t* const p_data, const unsigned int p_len);

				/// Send data
				/**
				*   \param[in] p_id ID of the connection to send the data
				*   \param[in] p_data unsigned byte vector of the data that must be send				
				*/
				void send(unsigned int p_id, std::vector<uint8_t>&& p_data);

				/// Broadcast data
				/**
				*	Send data to every connected socket
				*   \param[in] p_data unsigned byte array of the data that must be send
				*   \param[in] p_len length of the data array
				*/
				void broadcast(const uint8_t* const p_data, const unsigned int p_len);

				/// Broadcast data
				/**
				*	Send data to every connected socket
				*   \param[in] p_data unsigned byte vector of the data that must be send				
				*/
				void broadcast(std::vector<uint8_t>&& p_data);

				/// Disconnect
				/**
				*	Terminate the connection with one specific socket
				*   \param[in] p_id ID of the connection that should be closed
				*/
				void disconnect(unsigned int p_id);

				/// Recieve function
				/**
				*	Sets the function that should be called with every incomming message
				*   \param[in] p_func function pointer to recieve all messages 
				*/
				inline void setRecvFunc(std::function<void(unsigned int, std::vector<uint8_t>&&)> p_func)
				{
					m_functionMutex.lock();
					this->m_func_recv = p_func;
					m_functionMutex.unlock();
				}

				/// Manage connection function
				/**
				*	Sets the function that should be called with every new client
				*   \param[in] p_func function pointer to allow or discard individual connecting clients
				*/
				inline void setNewConFunc(std::function<bool(unsigned int, Socket*)> p_func)
				{
					m_functionMutex.lock();
					this->m_func_newConnection = p_func;
					m_functionMutex.unlock();
				}

			};

		}

	}
}

