/* packet_id.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once


namespace remote_control
{
	namespace  communication
	{

		
		namespace Server
		{
			enum SERVER_PACKET_ID
			{
				UNKNOWN = 0,
				KEEP_ALIVE,
				ACK,
				DATA,				// Some sort of information request
				STEERING_CARD,
				DISCARD_SHOWER,
				SHUTDOWN_CLIENT
			};
		}
		
		namespace Client
		{
			enum CLIENT_PACKET_ID
			{
				UNKNOWN = 0,
				KEEP_ALIVE,
				ACK,
				DATA,				// Some sort of information request
				CLIENT_REGISTER,	// Register Corsika instance at a server			
				START_SHOWER,		// Send when new shower is initialized with EVTH
				SHOWER,				// Send with aditional parameter  after partial shower simulation
				END_SHOWER,			// Send when all particles from shower are processed 
				SHUTDOWN
			};
		}
		
		enum DATA_PACKET_ID
		{
			UNKNWON = 0,
			WALLTIME,
			RUNTIME,
			TIME_CURRENT_EVENT,
			NUMBER_SIMULATED_EVENTS,	// Bisher simulierte events
			PARTICLE_CURRENT_EVENT,		// Simulierte Particle im shower		
			PHOTON_CURRENT_EVENT		// Simulierte photonen
		};

	}

}





