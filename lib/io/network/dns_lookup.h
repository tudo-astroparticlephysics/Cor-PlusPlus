/* dns_lookup.cpp
 * this file is part of Dynstack/RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */
#pragma once


#include <string>


namespace lib
{
	namespace io
	{
		namespace network
		{

			/// Converts a dns to the specific ip
			/**
			*   \param[in] p_data DNS of the target host
			*   \return ip of the host
			*/
			std::string hostname_to_ip(const std::string p_hostname);
		}
	}
}
