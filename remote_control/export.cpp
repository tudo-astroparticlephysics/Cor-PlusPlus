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

#include <memory>
#include <array>
#include <iostream>

#include <sstream>
#include <string>
#include <algorithm>

#include "user_code/remotecontrol_setup.h"

#include "remote_control/communication/packet.h"
#include "remote_control/control/main_control.h"

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
		list.push_back(item);
	}
	
	item = list.front();
	list.pop_front();

	remotecontrol_steering_card.emplace(item, list);
	
}

/** \details
 *
 */
void remotecontrol_start_()
{
	remotecontrol_setup();


	std::string ip;
	short port = 0;

	for (auto itr : remotecontrol_steering_card)
	{
		std::cout << "(RC) Setting: " << itr.first << " with ";
		for (auto tmp : itr.second)
		{
			std::cout << tmp << '\t';
		}
		std::cout << std::endl;

		if (itr.first == "REMOTECONTROL_IP")
		{
			if (itr.second.size() != 2)
			{
				std::cerr << "(RC) Invalid number of parameter for REMOTECONTROL_IP (dns) (port)" << std::endl;
				throw(std::exception());
			}

			std::stringstream sstr;

			ip = itr.second.front();
			itr.second.pop_front();

			sstr << itr.second.front();
			sstr >> port;
			itr.second.pop_front();
		}
		else if (itr.first == "REMOTECONTROL_P")
		{
			
		}
		else
		{
			std::cerr << "(RC) Filter in corsika.f are not correct or unsupported setting" << std::endl;
			std::cerr << "(RC) " << itr.first << std::endl;
			
		}
	}

	std::cout << "IP|" << ip << "|" << port << "|" << std::endl;
	remote_control::SMainControl().start(ip, port);
}

/** \details
 *
 */
void remotecontrol_end_()
{
	remote_control::SMainControl().stopp();
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
	remote_control::communication::Packet p(static_cast<uint32_t>(RUN_HEADER_ID), 273);
	p.append(data, 273);
	remote_control::SMainControl().send(p);
#endif
	(void) (data);
}

void remotecontrol_push_rune_(const float* data)
{
#ifdef SEND_RUN_END
	std::cout << "Send run end" << std::endl;
	remote_control::communication::Packet p(static_cast<uint32_t>(RUN_END_ID), 273);
	p.append(data, 273);
	remote_control::SMainControl().send(p);
#endif
	(void) (data);
}

void remotecontrol_push_evth_(const float* data)
{
#ifdef SEND_EVENT_HEADER
	std::cout << "Send event header " << EVENT_HEADER_ID << std::endl;
	remote_control::communication::Packet p(static_cast<uint32_t>(EVENT_HEADER_ID), static_cast<unsigned int>(273) );
	p.append(data, 273);
	remote_control::SMainControl().send(p);
#else
	(void) (data);
#endif
}

void remotecontrol_push_evte_(const float* data)
{
#ifdef SEND_EVENT_END
	std::cout << "Send event end" << std::endl;
	remote_control::communication::Packet p(static_cast<uint32_t>(EVENT_END_ID), 273);
	p.append(data, 273);
	remote_control::SMainControl().send(p);
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
	remote_control::communication::Packet p(static_cast<uint32_t>(ID), static_cast<unsigned int>(len));

	p.append(data, len);

	remote_control::SMainControl().send(p);

}

/** \details
 *  Function to receive steering card parameter. It returns the next parameter with every call. If the the line is empty (length = 0)
 *  the end of file is reached.
 */
void remotecontrol_steering_nextline_(char* line, int* lineLength, int* length)
{
	static int lineCounter = 0;

	static std::vector<std::string> input;
	if(input.size() == 0)
	{
		while (!std::cin.fail())
		{
			std::string tmp;
			std::getline(std::cin, tmp);
			input.push_back( tmp.erase(tmp.find_last_not_of(" \n\r\t")+1) );
		}
    		
	}
	

	std::fill(line, &line[*lineLength], ' ');
	if(static_cast<int>( input.size() ) > lineCounter)
	{
		std::string lineInput = input[lineCounter];
		

		*length = static_cast<int>(lineInput.length());
	
		std::replace(lineInput.begin(), lineInput.end(), '\t', ' ');

		if (static_cast<int>(lineInput.length()) <= *lineLength)
		{
			lineInput.copy(line, lineInput.length(), 0);
		}
		else
		{
			std::cerr << "Remotecontrol: Steeringcard line length is longer then allowed! (" << *lineLength << " bytes max)" << std::endl;
		}
	
		lineCounter++;
		return;
	}
	else
	{
		*length = 0;
		return;
	}
}

