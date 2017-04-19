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

#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>

#include <chrono>

#include "lib/io/network/client.h"

#include "remote_control/communication/message_queue.h"
#include "remote_control/communication/packet.h"

#include "remote_control/control/periodic_task.h"

#include "user_code/remotecontrol_setup.h"

namespace remote_control
{
	
	class MainControl
	{
	private:

		///
		/** \defgroup Thread Access only in main (thread) loop thread or with stopped thread
		 * @{
		 *
		 */
		lib::io::network::Client m_client;

		decltype(register_periodic_callback()) m_periodic;
		decltype(register_server_callback()) m_callback;

		/// @}


		///
		/** \defgroup Main Access only with from external sources
		 * @{
		 *
		 */
		std::thread m_thread;


		/// @}
		

		/// 
		/** \defgroup Synchronisation Communication with the parallel running network thread
		 *  @{
		 *   Every variable which is thread save is marked with a ts **in this class**
		 */
		std::atomic<bool> m_tsRunning;
		remote_control::communication::MessageQueue m_tsQueue;



		//std::mutex m_periodicMutex;
		//std::vector<>
		/// @}



		MainControl();

		void loop();

	public:

		virtual ~MainControl();

		inline static MainControl& getInstance()
		{
			static MainControl instance;
			return instance;
		}

		bool start(const std::string dns, const unsigned short port);
		void stopp();

		void send(remote_control::communication::Packet p);
	};

	inline static MainControl& SMainControl()
	{
		return MainControl::getInstance();
	}

} /* namespace RemoteControl */
