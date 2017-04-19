/* lifo_std_stack.h
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

#include "dynstack/stack/stack.h"

#include <deque>
#include <vector>

namespace dynstack
{
	namespace storage
	{

		/// Basic Last in First Out storage
		/**
		*	Implements a basic last in first out storage similar in functionality to the default corsika implementation.
		*	This class alone can not handle overflows for to many elements to store, it will simply remove them and returns -1 for the push_back routine.
		*	In most cases (x64) you can simply expand your stack to several gigabyte of memory and should never run out of memory, for all other cases 
		*	the overflow_to_disk_stack decorated should be the appropriated solution.
		*
		*	\tparam TType Type of the elements that should be stored into the memory. The data is stored directly as object and does not get casted.
		*/
		template<typename TType>
		class LIFO_std_Stack: public Stack<TType>
		{
			static_assert(!std::is_pointer<TType>::value, "Template parameter can't be a pointer!");

		private:

			std::vector<TType> m_tBuffer;			
			const unsigned int m_uiSize;

		protected:
			


		public:

			LIFO_std_Stack(const unsigned int size)
					: Stack<TType>(), m_uiSize(size)
			{
				m_tBuffer.reserve(size);

			}

			LIFO_std_Stack(LIFO_std_Stack<TType> && rhs)
					: Stack<TType>(), m_tBuffer( std::move(rhs.m_tBuffer) ), m_uiSize(rhs.m_uiSize)
			{								

			}

			~LIFO_std_Stack()
			{
			}



			inline int push_back(const TType& data)
			{
				if (m_tBuffer.size() < this->m_uiSize)
				{
					m_tBuffer.push_back(data);
					return 0;
				}
				return -1;
			}

			inline int push_back(TType&& data)
			{
				if (m_tBuffer.size()  < this->m_uiSize)
				{
					m_tBuffer.push_back(data);
					return 0;
				}
				return -1;
			}

			//Copys element to next free space
			inline int push_back(std::unique_ptr<const TType[]> data, const unsigned int elem)
			{
				if (this->m_tBuffer.size() <= this->m_uiSize - elem)
				{
					std::copy(data.get(), data.get() + elem, std::back_inserter(m_tBuffer));
					return 0;
				}
				return -1;
			}

			inline int push_back(const TType* const data, const unsigned int elem)
			{
				if (this->m_tBuffer.size()  <= this->m_uiSize - elem)
				{
					std::copy(data, data + elem, std::back_inserter(m_tBuffer));
					return 0;
				}
				return -1;
			}

			//Get last element without removing (copy)
			inline TType back()
			{
				if (m_tBuffer.size() > 0)
				{
					return m_tBuffer.back();
				}
				return TType();
			}

			//Get last element with removing (move)
			inline TType pop_back()
			{
				if (m_tBuffer.size() > 0)
				{
					TType tmp( m_tBuffer.back() );
					m_tBuffer.pop_back();

					return std::move(tmp);
				}
				return std::move(TType());
			}

			//Removes last element
			inline bool pop()
			{
				if (this->m_tBuffer.size() > 0)
				{
					m_tBuffer.pop_back();
					return true;
				}
				return false;
			}

			inline void clear()
			{
				m_tBuffer.resize(0);
			}


			//Get new empty element to write into (direct access, zero copy)
			inline StackPtr<TType> reserve()
			{
				return StackPtr<TType>();
			}

					

			inline unsigned long size() const
			{
				return (long)this->m_tBuffer.size();
			}

			inline unsigned long capacity() const
			{
				return m_uiSize;
			}

		};
	}

}



