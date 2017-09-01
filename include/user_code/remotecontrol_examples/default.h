/* default.h
 * this file is part of RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */



/// Contains default handling for basic message transfer and data logging.
/**
 *  This functions implement all messages that are needed to use the public remote server for logging purposes
 */



#pragma once

#include <chrono>

#include "remote_control/control/periodic_task.h"
#include "remote_control/communication/packet.h"


/// Periodic check if booth party's are still working properly
remote_control::PeriodicTask still_alive();


///
/** \defgroup System messages
* @{
*	Messages can request and send data.
*/

/// Default register to id 0x001
const remote_control::communication::Packet still_alive(const std::vector<uint8_t> p_data);

/// Default register to id 0x002
const remote_control::communication::Packet are_you_alive(const std::vector<uint8_t> p_data);

/// Default register to id 0x003
const remote_control::communication::Packet ack(const std::vector<uint8_t> p_data);

/// @}








///
/** \defgroup Information packets
* @{
*	Contains data from server to client
*/

/// Default register to id 0x101
const remote_control::communication::Packet get_steeringCard(const std::vector<uint8_t> p_data);


/// Default register to id 0x102
const remote_control::communication::Packet get_particle(const std::vector<uint8_t> p_data);


/// @}






///
/** \defgroup Query packets
* @{
*	Answeres requests from server. Usually the p_data part should be empty for default requests
*/

/// Default register to id 0x201
const remote_control::communication::Packet last_EVTH(const std::vector<uint8_t> p_data);

/// Default register to id 0x202
/**
 *  In normal cases there should only be one runh
 */
const remote_control::communication::Packet last_RUNH(const std::vector<uint8_t> p_data);

/// Default register to id 0x203
const remote_control::communication::Packet last_EVTE(const std::vector<uint8_t> p_data);

/// Default register to id 0x204
const remote_control::communication::Packet last_RUNE(const std::vector<uint8_t> p_data);

/// @}
