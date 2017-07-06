/* null_stack.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once

#include <type_traits>
#include <limits>

#include "dynstack/stack/stack.h"
#include "dynstack/stack/stack_ptr.h"

namespace dynstack
{
	namespace storage
	{
		template<typename TType>
		class Null_Stack: public Stack<TType>
		{
			static_assert(!std::is_pointer<TType>::value, "Template parameter can't be a pointer!");

		private:
			

		protected:			

		public:

			Null_Stack()
					: Stack<TType>()
			{
			
			}

			Null_Stack(Null_Stack<TType> && rhs)
					: Stack<TType>()
			{
				(void(rhs));						
			}

			~Null_Stack()
			{
				
			}

			inline int push_back(const TType& data)
			{
				(void(data));
				return 0;
			}

			inline int push_back(TType&& data)
			{
				(void(data));
				return 0;
			}

			//Copys element to next free space
			inline int push_back(std::unique_ptr<const TType[]> data, const unsigned int elem)
			{
				(void(data));
				(void(elem));
				return 0;
			}

			inline int push_back(const TType* const data, const unsigned int elem)
			{
				(void(data));
				(void(elem));
				return 0;
			}

			//Get last element without removing (copy)
			inline TType back()
			{
				return TType();
			}

			//Get last element with removing (move)
			inline TType pop_back()
			{
				return TType();
			}


			//Removes last element
			inline bool pop()
			{
				return true;
			}

			inline void clear()
			{

			}

			

			inline unsigned long size() const
			{
				return (long)0;
			}

			unsigned long capacity() const
			{
				return std::numeric_limits<unsigned long>::max();
			}

		};
	}

}
