/* in_modify_stack.h
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

		
		template<class TStack, typename TStack::m_ReturnType (*TFunc)(const typename TStack::m_ReturnType* const)>
		class InputModifyStack: public TStack
		{

			static_assert(std::is_base_of<_Stack, TStack>::value, "TStack must be a Stack!");
			
		private:

			typedef typename TStack::m_ReturnType TType;
			typedef typename TStack::m_StackType TTypeStack;


		protected:
			

		public:

			template<typename ... Args>
			InputModifyStack(Args&&... args)
					: TStack(args...)
			{
			}

			InputModifyStack(InputModifyStack<TStack, TFunc> && rhs)
				: TStack(std::forward<TStack>(rhs))
			{				
			}

			~InputModifyStack()
			{

			}

			inline int push_back(const TType& data)
			{
				return TStack::push_back( TFunc( &data) );
			}
			inline int push_back(TType&& data)
			{				
				return TStack::push_back( TFunc( &data) );
			}

			//Copys element to next free space
			inline int push_back(std::unique_ptr<const TType[]> data, const unsigned int elem)
			{				
				std::unique_ptr<TType[]> tmp(new TType[elem]);

				for (unsigned int i = 0; i < elem; i++)
				{					
					tmp[i] = TFunc(data.get() + i);					
				}
				return TStack::push_back(std::move(tmp), elem);
			}

			inline int push_back(const TType* const data, const unsigned int elem)
			{
				std::unique_ptr<TType[]> tmp(new TType[elem]);

				for (unsigned int i = 0; i < elem; i++)
				{					
					tmp[i] = TFunc(data + i);
				}
				return TStack::push_back(std::move(tmp), elem);
			}		

		};

	}
}


