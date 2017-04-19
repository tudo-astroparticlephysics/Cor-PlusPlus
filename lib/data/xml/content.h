/* content.h
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
#include <type_traits>



namespace lib
{
	namespace data
	{
		namespace xml
		{

			
			class Content 
			{
			private:
				const char* const m_cData;

				const unsigned int m_uiBytes;
			


			public:

				constexpr Content(const char* const p_ptr, const unsigned int p_size)
					:m_cData( p_ptr ), m_uiBytes(p_size)
				{

				}
				
			};
		}
	}

}

