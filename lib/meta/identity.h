/* identity.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once


#include <exception>
#include <type_traits>
#include <memory>


/**
*    \file identity.h
*    \brief Collection of different identity functions
*
*    A collection of different compile time resolvable identity functions mostly used in meta programing or as placeholder.
*/

namespace lib
{
	namespace meta
	{

		/// Identity structure
		/**
		*	 A callable structure that returns its input exactly and is known at compiletime		
		*/
		struct Identity
		{
			/// Identity function
			/**
			*	 A function that returns its input exactly and is known at compiletime
			*    \tparam U Type of input/return parameter
			*    \param[in] v Input parameter that gets forwarded
			*    \return Perfect forward of the input parameter v
			*/
			template<typename U>
			constexpr auto operator()(U&& v) const noexcept	-> decltype(std::forward<U>(v))
			{
				return std::forward<U>(v);
			}
		};


		/// Identity function
		/**
		*	 A function that returns its input exactly and is known at compiletime
		*    \tparam U Type of input/return parameter
		*    \param[in] v Input parameter that gets forwarded
		*    \return Perfect forward of the input parameter v
		*/
		template<typename U>
		constexpr auto id(const U&& v) noexcept -> decltype(std::forward<U>(v))
		{
			return std::forward<U>(v);
		}



		
	

	}

}
