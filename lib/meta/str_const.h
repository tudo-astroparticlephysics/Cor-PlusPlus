/* str_const.h
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
#include <exception>


namespace lib
{
	namespace meta
	{

		/// Compile time constant string
		class str_const
		{
		private:
			const char* const m_data;
			const unsigned int m_size;
		public:

			template<unsigned int TSize>
			constexpr str_const(const char(&a)[TSize]) :
			m_data(a), m_size(TSize - 1)
			{
			}

			constexpr char operator[](const unsigned int i)
			{
				return i < m_size ? m_data[i] : throw std::out_of_range("");
			}

			constexpr unsigned int size()
			{
				return m_size;
			}
		};

	}
}

