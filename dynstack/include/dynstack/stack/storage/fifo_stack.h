/* fifo_stack.h
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
#include <queue>

#include <type_traits>

#include "dynstack/stack/stack.h"
//#include "dynstack/stack/stack_ptr.h"

namespace dynstack
{
	namespace storage
	{

		/// First in - First out storage
		/**
		 *   Implemented as a wrapper arround std::queue the class provides a basic First in First out, stack compatible, storage.
		 *
		 *   \todo Replace std::queue<TType> with a efficient storage
		 *
		 *   \tparam TType Type of the content stored in the stack. For normal CORSIKA it should be of the type \ref ParticleData.
		 *   				It is important that the type is default constructible, capable and movable!
		 */
		template<typename TType>
		class FIFO_Stack: public Stack<TType>
		{
		private:
			/// Underlaying data structure that contains the storage
			std::unique_ptr< std::queue<TType> > m_tBuffer;

			/// Maximum size in elements that this stack is allowed to use and allocate
			const unsigned int m_uiSize;

		protected:

		public:

			/// Constructor
			/**
			 *
			 * \param[in] size Maximum number of elements that the queue is allowed to store.
			 * 				The storage is allocated in bunches so that actual storage can be more.
			 */
			FIFO_Stack(const unsigned int size)
					: Stack<TType>(), m_uiSize(size)
			{
				m_tBuffer = std::unique_ptr< std::queue<TType> >(new std::queue<TType>());
            }

			/// Copy constructor
			/**
			*  Copys the underlying storage container to the new object.
			*/
			FIFO_Stack(const FIFO_Stack<TType>& rhs)
				: Stack<TType>(), m_uiSize(rhs.m_uiSize)
			{
				m_tBuffer = std::unique_ptr< std::queue<TType> >(new std::queue<TType>(*rhs.m_tBuffer));
			}

			/// Move constructor
			/**
			 *  Moves the underlying storage container to the new object. The old stack loses its validity and can not be used anymore.
			 */
			FIFO_Stack(FIFO_Stack<TType> && rhs)
					: Stack<TType>(), m_tBuffer( std::move(rhs.m_tBuffer) ), m_uiSize(rhs.m_uiSize)
			{            
			}

			/// Destructor
			~FIFO_Stack()
			{
			}

			/// Store single element by copy
			/**
			 *  Checks if the stack is still valid and the size limit is meet, afterwards it
			 *  pushes the data on the underlying std::deque data structure.
			 *
			 *  \param[in] data Element to copy on the internal storage
			 *  \return Returns 0 if the data is successfully stored and -1 otherwise
			 */
		    inline int push_back(const TType& data)
			{
                //if constexpr( std::is_copy_constructible<TType>::value )  C++17 feature to partial enable support of not copyconstructable types
                //{
                    if (m_tBuffer->size() < m_uiSize)
    				{
    					m_tBuffer->push( data );
    					return 0;
    				}
                    return -1;
                //}
                //else
                //{
                //    return -1;
                //}

			}


			/// Store single element by move
			/**
			 *  Checks if the stack is still valid and the size limit is meet, afterwards it
			 *  pushes the data on the underlying std::deque data structure.
			 *
			 *  \param[in/out] data Element gets invalid when its moved into the function. The actual data gets copied on the stack.
			 *  \return Returns 0 if the data is successfully stored and -1 otherwise
			 */
			inline int push_back(TType&& data)
			{
				if (m_tBuffer->size() < m_uiSize)
				{
					m_tBuffer->push( std::move(data) );
					return 0;
				}
				return -1;
			}

			/// Stores an array of data to the stack
			/**
			 *  Checks if the stack is still valid and for each sequential insert if the size limit is meet.
			 *  The data is pushed in sequential order on the underlying std::deque data structure.
			 *
			 *  \param[in] data Element to store in an sequential order
			 *  \return Returns true if all data is successfully stored and false if no or only a part is stored
			 */
			inline int push_back(std::unique_ptr<const TType[]> data, const unsigned int elem)
			{
                //if constexpr( std::is_copy_constructible<TType>::value )  C++17 feature to partial enable support of not copyconstructable types
                //{
                    for (unsigned int i = 0; i < elem; i++)
                    {
    					if (m_tBuffer->size() >= m_uiSize)
    					{
    						return -1;
                        }
                        m_tBuffer->push(data[i]);
    				}
    				return 0;
                //}
                //else
                //{
                //    return -1;
                //}
			}

			/// Stores an array of data to the stack
			/**
			 *  Checks if the stack is still valid and for each sequential insert if the size limit is meet.
			 *  The data is pushed in sequential order on the underlying std::deque data structure.
			 *
			 *  \param[in] data Element to store in an sequential order
             *  \param[in] elem Number of elements stored in the Array
             *  \return Returns true if all data is successfully stored and false if no or only a part is stored
             */
             inline int push_back(const TType* const data, const unsigned int elem)
             {
                 //if constexpr( std::is_copy_constructible<TType>::value )  C++17 feature to partial enable support of not copyconstructable types
                 //{
                     for (unsigned int i = 0; i < elem; i++)
                     {
                         if (m_tBuffer->size() >= m_uiSize)
                         {
                             return -1;
                         }
                         m_tBuffer->push(data[i]);
                     }

                     return 0;
                //}
                // else
                // {
                //     return -1;
                //}
             }

			/// Copy of the earliest inserted element
			/**
			 *  For this storage a copy of the first inserted element is returned. In some cases (invalid or empty stack) an empty (default constructed) element is
			 *  returned. The user must check on its own if the element is valid, as example its possible to check the size beforehand.
			 *  The returned element is not removed from the stack and is still accessible after this call.
			 *
			 *  \return Returns a copy of the first element of the stack. If the stack is empty or invalid a default constructed element is returned
			 */
			inline TType back()
			{
				if (m_tBuffer == nullptr)
					return std::move(TType());

				if (m_tBuffer->size() > 0)
					return m_tBuffer->front();
				else
					return std::move(TType());

			}

			/// Get the earliest inserted element
			/**
			 *  For this storage the first inserted element is returned. In some cases (invalid or empty stack) an empty (default constructed) element is
			 *  returned. The user must check on its own if the element is valid, as example its possible to check the size beforehand.
			 *  The returned element is removed from the stack and is not accessible after this call.
			 *
			 *  \return Copy the earliest inserted element of the stack and moves it as return value. If the stack is empty or invalid a default constructed element is returned
			 */
			inline TType pop_back() //const
			{
				if (m_tBuffer == nullptr)
					return std::move(TType());

				if (m_tBuffer->size() > 0)
				{
					TType tmp( std::move(m_tBuffer->front()));
					m_tBuffer->pop();
					return std::move(tmp);
				}
				else
					return std::move( TType() );

			}

			/// Removes the earliest inserted element
			/**
			 *  This function removes the earliest inserted element of the storage. If the storage is empty or invalid no element is removed.
			 *
			 *  \return Returns true if an element is destroyed and false otherwise
			 */
			inline bool pop()
			{
				if (m_tBuffer == nullptr)
					return false;

				if (m_tBuffer->size() > 0)
					m_tBuffer->pop();
				else
					return false;

				return true;
			}

			/// Removes every element from stack
			/**
			*  This function removes all elements from stack and set everything back to zero
			*/
			inline void clear()
			{
				if (m_tBuffer)
					m_tBuffer.release();

				m_tBuffer = std::unique_ptr< std::queue<TType> >( new std::queue<TType>()) ;
			}

			/// Currently unsupported function
			/**
			 * Creates a new storage area like push_back but keeps it empty and does not copy anything into it. It returns the pointer to this storage area directly.
			 *
			 * The idea behind this function is direct (zero copy) write access to the underlying data structure. This can lead to a speed up in very IO heavy situations.
			 * Additional precautions must be taken that you don't overwrite the element or access already removed (pop) storage areas.
			 *
			 * A possible check for debug mode if all constrains are meet is the SaveStorage wrapper that allows runtime checks.
			 *
			 * \warning Advanced users only
 			 *
			 * \return Returns a pointer to new storage area without and precaution
			 */

			inline unsigned long size() const
			{
				if (m_tBuffer == nullptr)
					return 0;

				return (const unsigned long) m_tBuffer->size();
			}

			unsigned long capacity() const
			{
				return m_uiSize;
			}
		};
	}

}
