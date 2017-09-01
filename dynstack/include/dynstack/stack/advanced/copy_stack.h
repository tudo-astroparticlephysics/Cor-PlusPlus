/* copy_stack.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once

#include <memory>
#include <functional>
#include <assert.h>

#include <iostream>
#include <tuple>

#include "dynstack/stack/stack.h"

#include "meta/tuple/tuple_algorithm.h"

namespace dynstack
{
	namespace advanced
	{
		/// Copys element into two independet stacks
		/**
		*	This class copys all inserted elements into booth stacks. There is no possibility to remove pop any element from the secondary stack. Therefore
		*	the main purpose of this class is to stack further filters, callbacks or save to disk to allow a unified way to deal with this workload.
		*	Its possible to implement similiar method in a single lengthy callback routine.
		*
		*	Additional this class only works if the type of the main stack is _standard constructible and castable_ to the type of the copy stack.
		*
		*	\tparam TStackOrig	Main stack that all functionallity depends on
		*	\tparam TStackCopy	Stack that all pushed elements get additional copied to. Its not possible to querry any information to this class with
		*	this decorator.
		*/
		template<class TStackOrig, class TStackCopy>
		class CopyStack: public meta::tuple::tupleUnpack< TStackOrig >
		{

			static_assert(std::is_base_of<_Stack, TStackOrig>::value, "TStackOrig must be a Stack!");
			static_assert(std::is_base_of<_Stack, TStackCopy>::value, "TStackCopy must be a Stack!");

		private:

			typedef typename TStackOrig::m_ReturnType TType;
			typedef typename TStackOrig::m_StackType TTypeStack;


			meta::tuple::tupleUnpack< TStackCopy > m_oCopy;

		protected:


		public:

			template<typename ... TArgs1, typename ... TArgs2>
			CopyStack(std::tuple<TArgs1...> arg1, std::tuple<TArgs2...> arg2)
					: meta::tuple::tupleUnpack< TStackOrig >( std::forward<std::tuple<TArgs1...> >(arg1) ), m_oCopy(arg2)
			{
			}


			CopyStack(CopyStack<TStackOrig, TStackCopy> && rhs)
				: TStackOrig(std::forward<TStackOrig&&>(rhs) ), m_oCopy( std::move(rhs.m_oCopy) )
			{
			}

			~CopyStack()
			{

			}

			inline int push_back(const TType& data)
			{
				TType copy(data);
				m_oCopy.push_back(copy);

				return TStackOrig::push_back(data);
			}
			inline int push_back(TType&& data)
			{
				TType copy(data);
				m_oCopy.push_back(copy);

				return TStackOrig::push_back(data);
			}

			//Copys element to next free space
			inline int push_back(std::unique_ptr<const TType[]> data, const unsigned int elem)
			{
				std::unique_ptr<TType[]> copy(new TType[elem]);

				for (unsigned int i = 0; i < elem; i++)
				{
					copy[i] = data[i];
				}

				m_oCopy.push_back( std::move(copy) , elem);
				return TStackOrig::push_back( std::move(data), elem);
			}

			inline int push_back(const TType* const data, const unsigned int elem)
			{

				m_oCopy.push_back(data, elem);
				return TStackOrig::push_back(data, elem);
			}

			///TODO: Implement pop, back and pop_back



		};

	}
}
