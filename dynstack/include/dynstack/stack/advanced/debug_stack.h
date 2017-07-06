	/* debug_stack.h
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
	namespace advanced
	{

		/// Storage for debuging
		/**
		*   Counts the number of references each element has
		*   
		*/
		template<typename TType>
		struct saveStorage
		{
		private:
			unsigned int numberOfReference;
			TType m_data;

		public:
			constexpr saveStorage(TType x)
					: numberOfReference(0), m_data(x)
			{
			}

			saveStorage()
					: numberOfReference(0)
			{
			}

			saveStorage& operator=(TType x)
			{
				numberOfReference = 0;
				m_data = x;
				return *this;
			}

			operator TType&()
			{
				return m_data;
			}
			operator TType() const
			{
				return m_data;
			}
			operator TType*()
			{
				return &m_data;
			}

			template<class T>
			operator T() const = delete;

			inline TType* data()
			{
				return &m_data;
			}
			
			inline unsigned int& references()
			{
				return numberOfReference;
			}

			inline unsigned int references() const
			{
				return numberOfReference;
			}
		};

		/// Stack for debuging 
		/**
		*   Debug stack that counts each reference. Currently not usefull, but for future zero copy access interesting
		*
		*/
		template<class TType, template<typename > class TStack>
		class DebugStack: public Stack<TType, saveStorage<TType>>
		{
			static_assert(std::is_base_of<_Stack, TStack<TType>>::value, "TStack must be a Stack!");

		private:

			typedef TStack<saveStorage<TType>> Storage;

			std::unique_ptr<Storage> m_stack;

		protected:
			
		public:

			template<typename ... Args>
			DebugStack(Args&&... args)
					: Stack<TType, saveStorage<TType>>()
			{
				m_stack = std::unique_ptr<Storage>(new Storage(args...));
			}

			DebugStack(DebugStack<TType, TStack> && rhs)
				: m_stack(std::move(rhs.m_stack))
			{
			}

			~DebugStack()
			{

			}

			inline int push_back(const TType& data)
			{
				return m_stack->push_back(saveStorage<TType>(data));
			}
			inline int push_back(TType&& data)
			{
				return m_stack->push_back(saveStorage<TType>(data));
			}

			//Copys element to next free space
			inline int push_back(std::unique_ptr<const TType[]> data, const unsigned int elem)
			{
				int check = 0;
				for (unsigned int i = 0; i < elem; i++)
				{
					check = check | m_stack->push_back(saveStorage<TType>(data[i]));
				}
				return check;
			}

			inline int push_back(const TType* const data, const unsigned int elem)
			{
				int check = 0;
				for (unsigned int i = 0; i < elem; i++)
				{
					check = check | m_stack->push_back(saveStorage<TType>(data[i]));
				}
				return check;
			}

			//Get last element without removing (copy)
			inline TType back()
			{
				if (m_stack->size() == 0)
				{
					return TType();
				}

				saveStorage<TType> data = m_stack->back();					

				return static_cast<TType>(data);
			}

			//Get last element with removing (move)
			inline TType pop_back()
			{
				if (m_stack->size() == 0)
				{
					return std::move(TType());
				}

				saveStorage<TType> data = m_stack->pop_back();
				
				return static_cast<TType>(data);
			}

			//Removes last element
			inline bool pop()
			{
				const saveStorage<TType> data = m_stack->back();

				//assert(data.numberOfReference == 0 && "Stack pop of element with still living references, possible access violation to deleted elements");

				if (data.references() == 0)
				{					
					return m_stack->pop();
				}
				std::cerr << "Tried Stack pop with still living references, possible access violation to deleted elements" << std::endl;
				std::cerr << "\t.clear() any used StackPtr or remove SecureStack" << std::endl;

				return false;
			}

			inline void clear()
			{
				m_stack->clear();
			}

			//Get new empty element to write into (direct access, zero copy)
			StackPtr<TType,saveStorage<TType>> reserve()
			{
				auto data = m_stack->back();
				data.references()++;

				/*void(*del)(saveStorage<TType>* const)  = [](saveStorage<TType>* const data){
					data->references()--;
				};*/

				return StackPtr<TType, saveStorage<TType>>();//StackPtr<TType,saveStorage<TType>>( std::move(data), del );
			}

			

			inline unsigned long size() const
			{
				return m_stack->size();
			}

			unsigned long capacity() const
			{
				return m_stack->capacity();
			}

		};

	}
}


