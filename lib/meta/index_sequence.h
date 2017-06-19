/* index_sequence.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once


#include<type_traits>
#include<tuple>


/**
*    \file index_sequence.h
*    \brief Implementation of the C++14 integer sequence
*
*    Reimplementation of the new C++14 integer_sequence from <utility>. It allows the creation of number sequences at compile time and is only used in meta programming.
*    One common usecase is the access to multiple elements of a parameter pack or std::tuple at compile time
*
*    How to use:
*    @code template<size_t ... TArgs> void f(integer_sequence<TArgs>) @endcode
*    with the calling class @code f(index_sequence_from<4>); @endcode
*    This creates the compile time list 4,3,2,1,0 for the function to use
*    
*/

namespace lib
{
	namespace meta
	{

		/// Placeholder to capture the template list and use it
		/**
		*    Used as function parameter to capture the template parameter for further use
		*    
		*    \tparam Ints Compile time list of integers
		*/
		template<size_t... Ints>
		struct integer_sequence {};

		template<size_t Size, size_t End, size_t ... Ints>
		struct implementation;

		template<size_t Size, size_t End, size_t... Ints>
		struct implementation : implementation< Size - 1, End, End + Size - 1, Ints...>
		{};

		template<size_t End, size_t... Ints>
		struct implementation<0, End, Ints...>
		{
			typedef integer_sequence<Ints...> type;
		};


		/// Index list for a variadic template
		/**
		*    Creates an index list from (size - 1) to 0. 
		*    \warning It is not save to call this with an empty parameter pack
		*    \tparam T Parameter list to create indices from, only the size of T is actually used
		*/
#if __cplusplus > 199711L
		template<class... T>
		using index_sequence_for = typename implementation<sizeof...(T) - 1, 0>::type;
#else
		template<class... T>
		struct index_sequence_for
		{
			typedef typename implementation<sizeof...(T) - 1, 0>::type type;
		};
#endif

		/// Index list for a spezific number
		/**
		*    Creates an index list from TStart to 0.
		*    \warning It is not save to call this with TStart<=0
		*    \tparam TStart Startparameter for the indice list
		*/
#if __cplusplus > 199711L		
		template<size_t TStart>
		using index_sequence_from = typename implementation<TStart, 0>::type;
#else
		template<size_t TStart>
		struct index_sequence_from
		{
			typedef typename implementation<TStart, 0>::type type;
		};
#endif


		/// Index list for a spezific number
		/**
		*    Creates an index list from TBegin to TEnd. 
		*    If booth parameter are the same it will only include this one indice in the list.
		*    \warning It is not save to call this with TStart<=0
		*    \warning TEnd must be bigger or equal with TBegin
		*    \tparam TBegin Startparameter for the indice list
		*    \tparam TEnd   Endparamter for the indice list
		*/
#if __cplusplus > 199711L		
		template<size_t TBegin, size_t TEnd>
		using index_sequence_from_to = typename implementation<TEnd - TBegin, TBegin>::type;
#else
		template<size_t TBegin, size_t TEnd>
		struct index_sequence_from_to
		{
			typedef typename implementation<TEnd - TBegin, TBegin>::type type;
		};
#endif


	}

}
