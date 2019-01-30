/* main_control.h
 * this file is part of Dynstack/RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#pragma once

#include "zmq.hpp"

#include <queue>

#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>




namespace remote_control
{

	class MainControl
	{
	private:		
		/// Storage of messages send to the server
		std::deque<zmq::message_t> m_msgBuffer;
		std::deque<zmq::message_t> m_recvBuffer;

		std::mutex m_mutex;
		std::condition_variable m_condition;


		std::atomic<bool> m_running;

		std::thread m_loop;

		std::string m_address;


		MainControl();
	
		void loop();

	public:
	
		virtual ~MainControl();

		inline static MainControl& getInstance()
		{
			static MainControl instance;
			return instance;
		}

		bool start(const std::string address);
		void stop();

		int send(const void* p, unsigned int len);
		int send(std::vector<char> p);
		
		std::vector<char> recv();
	};

	inline static MainControl& SMainControl()
	{
		return MainControl::getInstance();
	}

} /* namespace RemoteControl */
