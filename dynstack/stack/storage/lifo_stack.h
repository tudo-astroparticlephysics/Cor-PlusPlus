/* lifo_stack.h
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
#include "dynstack/stack/stack_ptr.h"

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
		class LIFO_Stack: public Stack<TType>
		{
			static_assert(!std::is_pointer<TType>::value, "Template parameter can't be a pointer!");

		private:

			std::unique_ptr<TType[]> m_tBuffer;
			
			TType* m_tNextFree;				// Pointer to next Empty position

			const unsigned int m_uiSize;

		protected:
			// Current Offset in Elements
			inline unsigned long getCurrentOffset() const
			{
				return (long)(m_tNextFree - m_tBuffer.get());
			}


		public:

			LIFO_Stack(const unsigned int size)
					: Stack<TType>(), m_tBuffer(new TType[size]), m_uiSize(size)
			{
				m_tNextFree = m_tBuffer.get();

			}

			LIFO_Stack(LIFO_Stack<TType> && rhs)
					: Stack<TType>(), m_tBuffer( std::move(rhs.m_tBuffer) ), m_uiSize(rhs.m_uiSize)
			{								
				this->m_tNextFree = rhs.m_tNextFree;
				rhs.m_tNextFree = nullptr;

			}

			~LIFO_Stack()
			{
			}



			inline int push_back(const TType& data)
			{
				if (this->getCurrentOffset() < this->m_uiSize)
				{
					*m_tNextFree = data;

					m_tNextFree += 1;

					return 0;
				}
				return -1;
			}

			inline int push_back(TType&& data)
			{
				if (this->getCurrentOffset() < this->m_uiSize)
				{
					*m_tNextFree = TType( data );
					m_tNextFree += 1;

					return 0;
				}
				return -1;
			}

			//Copys element to next free space
			inline int push_back(std::unique_ptr<const TType[]> data, const unsigned int elem)
			{
				if (this->getCurrentOffset() <= this->m_uiSize - elem)
				{
					std::memcpy(m_tNextFree, data.get(), elem * sizeof(TType));
					m_tNextFree += elem;

					return 0;
				}
				return -1;
			}

			inline int push_back(const TType* const data, const unsigned int elem)
			{
				if (this->getCurrentOffset() <= this->m_uiSize - elem)
				{
					std::memcpy(m_tNextFree, data, elem * sizeof(TType));
					m_tNextFree += elem;

					return 0;
				}
				return -1;
			}

			//Get last element without removing (copy)
			inline TType back()
			{
				const unsigned long off = getCurrentOffset();
				if (off > 0)
				{
					return *(m_tNextFree - 1);
				}
				return TType();
			}

			//Get last element with removing (move)
			inline TType pop_back()
			{
				const unsigned long off = getCurrentOffset();
				if (off > 0)
				{
					TType tmp(std::move( *(m_tNextFree - 1) ));
					m_tNextFree = m_tNextFree - 1;

					return std::move(tmp);
				}
				return std::move(TType());
			}

			//Removes last element
			inline bool pop()
			{
				if (this->getCurrentOffset() > 0)
				{
					m_tNextFree = m_tNextFree - 1;
					return true;
				}
				return false;
			}

			inline void clear()
			{
				m_tNextFree = m_tBuffer.get();
			}


			//Get new empty element to write into (direct access, zero copy)
			inline StackPtr<TType> reserve()
			{
				const unsigned long off = getCurrentOffset();
				if (off < this->m_uiSize - 1)
				{
					m_tNextFree++;

					return Stack<TType>::createStackObj(m_tNextFree - 1);
				}
				return StackPtr<TType>();
			}

					

			inline unsigned long size() const
			{
				return (long)this->getCurrentOffset();
			}

			inline unsigned long capacity() const
			{
				return m_uiSize;
			}

		};
	}

}



