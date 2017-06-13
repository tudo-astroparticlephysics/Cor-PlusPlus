/* setup.h
 * this file is part of RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#pragma once

#include "remote_control/control/periodic_task.h"
#include "remote_control/communication/packet.h"

#include <array>
#include <iostream>
#include <functional>
#include <map>

#define SEND_EVENT_HEADER 1
#define EVENT_HEADER_ID 0x03

#define SEND_EVENT_END 1
#define EVENT_END_ID 0x04

#define SEND_RUN_HEADER 1
#define RUN_HEADER_ID 0x05

#define SEND_RUN_END 1
#define RUN_END_ID 0x06






// With feature level C++14 the decltype becomes uneccessary
inline auto register_periodic_callback() -> const std::array<remote_control::PeriodicTask, 1>
{
	/** User code here
	 *	Create your periodic tasks with lambda or a function pointer and the spacing.
	 *	The functions must execute fast, otherwise the internal message queue will stop working properly.
	 */

	remote_control::PeriodicTask empty([](){return;}, std::chrono::milliseconds(5000) );


	
	std::array<remote_control::PeriodicTask, 1> arr = {{ empty }}; 

	return  arr;//std::array<remote_control::PeriodicTask, 1>({ empty }); 
}


inline auto register_server_callback() -> const std::map<uint32_t, std::function<remote_control::communication::Packet(const std::vector<uint8_t>)>>
{
	typedef std::function<remote_control::communication::Packet(const std::vector<uint8_t>)> callback_type;
	std::map<uint32_t, callback_type > callback;


	/** User code here
	 *	Add user defined callbacks for messages send by the server
	 *	For best compatibility use packed id's of type 0x300 or greater
	 *	Lower values are used for default handling (0x0XX - System, 0x1XX - Information, 0x2XX - Requests)
	 *
	 *  Returned values
	 */

	auto call1 = [](std::vector<uint8_t> msg) -> remote_control::communication::Packet
			{
				///Check vector content
				(void)(msg);

				return remote_control::communication::Packet();
			};


	callback[0x301] = call1;


	return callback;

}

inline void remotecontrol_setup()
{
	/** User code here
	 *  Initialization of custom classes and function goes here
	 */



	return;
}



