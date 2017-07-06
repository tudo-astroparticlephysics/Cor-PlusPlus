/* out_callback_stack.h
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
		
		template<class TStack, void (*TFunc)(const typename TStack::m_ReturnType* const)>
		class OutputCallbackStack: public TStack
		{

			static_assert(std::is_base_of<_Stack, TStack>::value, "TStack must be a Stack!");

		private:
			
			typedef typename TStack::m_ReturnType TType;
			typedef typename TStack::m_StackType TTypeStack;

		protected:
			
		public:

			template<typename ... Args>
			OutputCallbackStack(Args&&... args)
					: TStack( std::forward<Args>(args)... )
			{
			}

			OutputCallbackStack(OutputCallbackStack<TStack, TFunc> && rhs)
					: TStack((TStack&&) rhs)
			{
			}

			~OutputCallbackStack()
			{

			}


			//Get last element without removing (no copy)
			TType back()
			{
				auto tmp = TStack::back();
				
				TFunc(&tmp);

				return tmp;
			}

			//Get last element without removing (no copy)
			TType pop_back()
			{
				auto tmp = TStack::pop_back();

				TFunc(&tmp);

				return tmp;
			}

		};

	}
}



