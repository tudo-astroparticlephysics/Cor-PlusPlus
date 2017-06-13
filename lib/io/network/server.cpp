/* server.cpp
 * this file is part of Dynstack/RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#include "lib/io/network/server.h"

#include <stdexcept>
#include <thread>
#include <chrono>

#if __cplusplus > 199711L
#else
#include <unistd.h>
#endif

namespace lib
{
	namespace io
	{

		namespace network
		{

			void Server::handleConnection()
			{
				while (m_isRunning.load() == true)
				{
					this->m_arb_listenSock.listen();
					auto list = lib::io::network::Socket::waitFor_read({ &m_arb_listenSock }, 1000);

					if (list.size() == 0)
						continue;

					std::unique_ptr<Socket> tmp(new Socket());
					this->m_arb_listenSock.accept(*tmp.get());

					const int sockID = m_nextID.fetch_add(1);

					if (this->m_func_newConnection(sockID, tmp.get()))
					{
						m_vectorMutex.lock();
						
						this->m_std_connectionSock.insert(std::make_pair(sockID, std::move(tmp)));						

						m_vectorMutex.unlock();
					}
				}
			}

			void Server::handleRecv()
			{
				while (m_isRunning.load() == true)
				{

					this->m_vectorMutex.lock();

					std::vector<lib::io::network::Socket*> waitList;

					for (auto itr = m_std_connectionSock.begin(); itr != m_std_connectionSock.end(); itr++)
					{
						waitList.push_back(itr->second.get());
					}
					this->m_vectorMutex.unlock();

					if (waitList.size() > 0)
					{
						auto recvReady = ::lib::io::network::Socket::waitFor_read(waitList, 50);

						for (auto itr : recvReady)
						{
							std::vector<uint8_t> data = itr->recv();
							m_functionMutex.lock();
							m_func_recv(0, std::move(data));
							m_functionMutex.unlock();
						}
					}
#if __cplusplus > 199711L
					std::this_thread::sleep_for(std::chrono::milliseconds(20));
#else
					usleep(20000);
#endif
					this->m_actionMutex.lock();					
				
					for (unsigned int i = 0; i < m_action.size(); i++)
					{
						m_action.back()(&m_std_connectionSock);						
						m_action.pop_back();
					}
					
					this->m_actionMutex.unlock();				
					
#if __cplusplus > 199711L
					std::this_thread::sleep_for(std::chrono::milliseconds(20));
#else
					usleep(20000);
#endif
				}
			}

			Server::Server(const unsigned short p_port)
			{
				m_nextID = 1;

				this->m_arb_listenSock.create();
				if( !this->m_arb_listenSock.bind(p_port) )
				{
					std::stringstream sstr;
					sstr << p_port;
					throw( std::runtime_error("Socket could not be bind to port" + sstr.str() ) );				
				}

				m_isRunning = true;
				m_threadNewCon = new std::thread(&Server::handleConnection, this);
				m_threadNewData = new std::thread(&Server::handleRecv, this);
			}

			Server::~Server()
			{
				m_isRunning.exchange(false);
				this->m_threadNewCon->join();
				this->m_threadNewData->join();

				this->m_arb_listenSock.close();			

				this->m_std_connectionSock.clear();

				delete this->m_threadNewCon;
				delete this->m_threadNewData;
			}

			void Server::send(unsigned int p_id, const uint8_t* const p_data, const unsigned int p_len)
			{
				auto func = [](connectionHandler* const con, unsigned int id, const uint8_t* const data, const unsigned int len)->void
						{							
							con->at(id)->send(data, len);
						};

				action f = std::bind(func, std::placeholders::_1, p_id, p_data, p_len);

				std::unique_lock<std::mutex> lock(this->m_actionMutex);
				m_action.push_back(f);
			}

			void Server::send(unsigned int p_id, std::vector<uint8_t>&& p_data)
			{
				this->send(p_id, p_data.data(), static_cast<const unsigned int>(p_data.size()) );
			}

			void Server::broadcast(const uint8_t* const p_data, const unsigned int p_len)
			{				
				auto func = [](connectionHandler* const con, const uint8_t* const data, const unsigned int len)->void{					
					for (auto& itr : *con)
					{
						itr.second->send(data, len);
					}
				};

				action f = std::bind(func, std::placeholders::_1, p_data, p_len);

				std::unique_lock<std::mutex> lock(this->m_actionMutex);
				m_action.push_back(f);
			}

			void Server::broadcast(std::vector<uint8_t>&& p_data)
			{
				this->broadcast(p_data.data(), static_cast<const unsigned int>(p_data.size()) );
			}

			void Server::disconnect(unsigned int p_id)
			{
				auto func = [](connectionHandler* const con, unsigned int id)->void
						{							
							con->erase(id);
						};

				action f = std::bind(func, std::placeholders::_1, p_id);

				std::unique_lock<std::mutex> lock(this->m_actionMutex);
				m_action.push_back(f);
			}

		}
	}
}
