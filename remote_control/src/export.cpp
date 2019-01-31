/* export.cpp
 * this file is part of RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#include "remote_control/export.h"

#include "remote_control/communication/packet.h"
#include "remote_control/control/main_control.h"

#include "user_code/remotecontrol_setup.h"


#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include <list>


static std::map<std::string, std::list<std::string> > remotecontrol_steering_card;

/** \details
 *
 */
void remotecontrol_init_(const char* line, const int* size)
{
	std::stringstream sstr(std::string(line, *size));

	std::list<std::string> list;
	std::string item;
	while (std::getline(sstr, item, ' '))
	{
		if(item == " " || item == "")
		{
			continue;
		}
		else
		{
			list.push_back(item);
		}
	}


	item = list.front();
	list.pop_front();

	remotecontrol_steering_card[item] = list;
}

/** \details
 *
 */
void remotecontrol_start_()
{
	if( remotecontrol_steering_card.size() == 0)
	{		
		std::cerr << "RC Steering card empty!" << std::endl;
		exit(-1337);
	}

	if( remotecontrol_steering_card.find("RC_IP") == remotecontrol_steering_card.end() )
	{
		std::cerr << "RemoteIP not found in steering card" << std::endl;
	}

	remote_control::SMainControl().start( remotecontrol_steering_card["RC_IP"].front() );
}

/** \details
 *
 */
void remotecontrol_end_()
{
	remote_control::SMainControl().stop();
}

//@{
/** \details
 *  The functions get called from CORSIKA when new information are available
 *  The data is converted into its C++ structure and the matching function from the user changeable setup file is called
 */
void remotecontrol_push_runh_(const float* data)
{
#ifdef SEND_RUN_HEADER
	std::cout << "Send run header" << std::endl;
	
	remote_control::SMainControl().send( remote_control::communication::Packet(123, data, 273).getData() );
#endif
	(void) (data);
}

void remotecontrol_push_rune_(const float* data)
{
#ifdef SEND_RUN_END
	std::cout << "Send run end" << std::endl;
	
	remote_control::SMainControl().send( remote_control::communication::Packet(124, data, 273).getData() );
#endif
	(void) (data);
}

void remotecontrol_push_evth_(const float* data)
{
#ifdef SEND_EVENT_HEADER
	std::cout << "Send event header " << EVENT_HEADER_ID << std::endl;
	
	remote_control::SMainControl().send( remote_control::communication::Packet(125, data, 273).getData() );
#else
	(void) (data);
#endif
}

void remotecontrol_push_evte_(const float* data)
{
#ifdef SEND_EVENT_END
	std::cout << "Send event end" << std::endl;
	
	remote_control::SMainControl().send( remote_control::communication::Packet(126, data, 273).getData() );
#endif
	(void) (data);
}
//@}

/** \details
 *  Is called every time CORSIKA creates a new initial particle for the shower simulation.
 */

void remotecontrol_push_initalparticle_(const float* data, unsigned int len)
{
	(void) (data);
	(void) (len);
}

/** \details
 *  Is called every time CORSIKA wants to create a new initial particle for the shower simulation.
 *  If len is 0 CORSIKA produces samples from the normal distribution defined in the inputcard
 */
void remotecontrol_recv_initalparticle_(const float* data)
{
	(void) (data);
}

/** \details
 *  Function that allows direct sending of a data packet to the server.
 *  The created data packet is added to a internal buffer and send as soon as possible.
 */
void remotecontrol_send_(const int ID, const void* data, int len)
{
	std::cout << "Normal send: " << ID << std::endl;
	

}
