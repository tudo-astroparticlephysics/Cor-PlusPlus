/* out_modify_stack.h
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

#include "dynstack/stack/stack.h"

namespace dynstack
{
	namespace wrapper
	{
		
		template<class TStack, void (*TFunc)(typename TStack::m_ReturnType&)>
		class OutputModifyStack: public TStack
		{

			static_assert(std::is_base_of<_Stack, TStack>::value, "TStack must be a Stack!");

		private:
			
			typedef typename TStack::m_ReturnType TType;
			typedef typename TStack::m_StackType TTypeStack;

		protected:
			
		public:

			template<typename ... Args>
			OutputModifyStack(Args&&... args)
					: TStack(args...)
			{
			}

			OutputModifyStack(OutputModifyStack<TStack, TFunc> && rhs)
					: TStack((TStack) rhs)
			{
			}

			~OutputModifyStack()
			{

			}

			//Get last element without removing (no copy)
			TType back()
			{
				auto tmp = TStack::back();
				
				TFunc(*tmp);

				return tmp;
			}

			TType pop_back()
			{
				auto tmp = TStack::pop_back();

				TFunc(*tmp);

				return tmp;
			}

		};

	}
}


