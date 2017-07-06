/* steering_card.h
 * this file is part of RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#pragma once

#include <list>
#include <string>
#include <map>

namespace lib
{
	namespace data
	{
		namespace corsika
		{

/// Easy write and read for Corsika steering cards
			/**
			 *	This class read and write steering cards. Additional easy access is granted to individual settings
			 *
			 */
			class steering_card
			{
			private:
				std::list<std::string> m_oSteeringList;

			public:

				typedef std::list<std::string>::iterator setting_iterator;

				steering_card();

				/// Reads one or multiple steering options separated with '\n' (newline) and add them to the internal storage.
				void insert(const std::string);

				/// Returns a map of type option -> attributes for convenient access
				const std::map<std::string, std::list<std::string> > get_settings();

				/// Iterator to the first element
				const setting_iterator begin();

				/// Iterator to the last element
				const setting_iterator end();

				/// Clears the internal storage and all settings
				void clear();

			};

		}
	}
}

