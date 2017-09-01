/* filter_stack.h
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
		/// Filters user defined elements
		/**
		*	A user defined filter class that removes specific elements from propagating further down the stack. This leads in normal usecase to
		*	the removal of the element.
		*
		*	\tparam TStack Type of the stack that should get decorated with the filter functionality. Is the element filtered (removed) this stacks will never
		*	see the pushed element.
		*	\tparam TFunc Function to check if a spezific element should be removed from the stack. If the function returnes 0 the inserted element
		*	is processed the normal way. Every return value not equal zero removes the element from further propagating the stack and the return value
		*	is returned to the push_back caller.
		*/
		template<class TStack, int(*TFunc)(const typename TStack::m_ReturnType* const)>
		class FilterStack: public TStack
		{
			static_assert(std::is_base_of<_Stack, TStack>::value, "TStack must be a Stack!");

		private:
			typedef typename TStack::m_ReturnType TType;
			typedef typename TStack::m_StackType TTypeStack;


			inline int filter(const TType* const data) const
			{
				return TFunc(data);
			}

		protected:
			

		public:

			typedef bool(*filter_function_type)(const typename TStack::m_ReturnType* const);
			
			template<typename ... TArgs>
			FilterStack(TArgs&&... args)
					: TStack(std::forward<TArgs>(args)...)
			{
			}

			FilterStack(FilterStack<TStack, TFunc> && rhs)
				: TStack((TStack)rhs)
			{				
			}

			~FilterStack()
			{

			}

			inline int push_back(const TType& data)
			{
				const int ret = filter(&data);
				if (ret == 0)
				{
					return TStack::push_back(data);
				}					
				else
				{
					return ret;
				}
					
			}
			inline int push_back(TType&& data)
			{
				const int ret = filter(&data);
				if (ret == 0)
				{
					return TStack::push_back(data);
				}					
				else
				{
					return ret;
				}					
			}

			//Copys element to next free space
			inline int push_back(std::unique_ptr<const TType[]> data, const unsigned int elem)
			{
		
				bool check = true;
				for (unsigned int i = 0; i < elem; i++)
				{
					const int ret = filter(data.get() + i);
					if (ret == 0)
					{
						const int tmpCheck = TStack::push_back(data[i]);
						check = check && tmpCheck;
					}
				}
				return check;
			}

			inline int push_back(const TType* const data, const unsigned int elem)
			{
				bool check = true;
				for (unsigned int i = 0; i < elem; i++)
				{
					if (filter(data + i) == 0)
					{
						check = check && TStack::push_back(data[i]);
					}
						
				}
				return check;
			}			

		};

	}
}



