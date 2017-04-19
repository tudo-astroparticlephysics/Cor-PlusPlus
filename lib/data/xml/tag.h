/* tag.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once

#include <cstring>
#include <string>
#include <sstream>
#include <vector>


#include "lib/data/xml/content.h"


namespace lib
{
	namespace data
	{
		namespace xml
		{			
			class Tag 
			{
			private:
				const std::vector<Content> m_oContent;

				const std::string m_sName;

			protected:
			};

		}
	}
} 


