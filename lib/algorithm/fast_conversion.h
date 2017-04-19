/* fast_conversion.h
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
	namespace algorithm
	{

		/// Fast conversion to string
		/**
		*   Generic conversion function
		*	\tparam T Type of parameter to convert
		*   \param obj Object contains the data to convert
		*/
		template<class TType>
		inline const std::string toString(const TType& p_obj);

		/// Fast conversion to string (float specialisation)
		/**
		*   fast conversion from float to string
		*	\param obj Object contains the data to convert 
		*/
		template<>
		inline const std::string toString<float>(const float& p_obj)
		{
			// http://dl.acm.org/citation.cfm?doid=1809028.1806623
			// https://github.com/miloyip/rapidjson/
			return "";
		}
	}
}






