/* message_queue.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once


#include <deque>

#include <mutex>
#include <condition_variable>
#include <atomic>
#include <memory>

#include "remote_control/communication/packet.h"

namespace remote_control
{
	namespace communication
	{


		class MessageQueue 
		{
		public:
			typedef ::remote_control::communication::Packet storage_type;

		private:

			std::deque< storage_type > m_queue;
			std::mutex m_mutex;

			std::condition_variable m_cv;
			std::atomic<uint32_t> m_size;

		protected:




		public:
			MessageQueue();
			virtual ~MessageQueue();


			void push_back(storage_type data);
			void push_front(storage_type data);

			storage_type pop_back_blocking();
			storage_type pop_front_blocking();

			bool empty();

		};
	}

} /* namespace lib */

