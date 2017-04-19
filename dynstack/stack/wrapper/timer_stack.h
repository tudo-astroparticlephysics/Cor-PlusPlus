/* in_callback_stack.h
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
#include <chrono>


#include "dynstack/stack/stack.h"

namespace dynstack
{
	namespace wrapper
	{
		/// Time measure wrapper
		/**
		*   Implemented is a wrapper that logs the runtime of different stack operations
		*
		*   \tparam TStack Stack to wrap
			\tparam TFunc Recieves every clear() functioncall all time data in the following order:
					Insert time normal push_back
					Insert time move push_back
					Insert time unique_ptr push_back
					Insert time raw pointer push_back
					Recieve time back
					Recieve time pop_back
					Time since last clear
		*/
		template<class TStack, void(*TFunc)(std::array<std::chrono::high_resolution_clock::duration, 7>, std::array<unsigned int, 6>) >
		class TimerStack: public TStack
		{

			static_assert(std::is_base_of<_Stack, TStack>::value, "TStack must be a Stack!");
			
		private:

			typedef typename TStack::m_ReturnType TType;
			typedef typename TStack::m_StackType TTypeStack;

			
			std::chrono::high_resolution_clock::time_point m_oStackCreate;

			/** Internal timer memory
			*
			*/
			std::array<std::chrono::high_resolution_clock::duration, 7> m_oTime;
			std::array<unsigned int, 6> m_oCounter;

		protected:

		public:

			template<typename ... Args>
			TimerStack(Args&&... args)
					: TStack( std::forward<Args>(args)... )
			{
				for (int i = 0; i < m_oCounter.size(); i++)
				{
					m_oCounter[i] = 0;
				}
				
				m_oStackCreate = std::chrono::high_resolution_clock::now();
			}
			
			TimerStack(TimerStack<TStack, TFunc> && rhs)
				: TStack((TStack&&)rhs), m_oStackCreate(rhs.m_oStackCreate)
			{								
			}

			~TimerStack()
			{

			}

			inline int push_back(const TType& p_data)
			{				
				auto start = std::chrono::high_resolution_clock::now();
				auto ret = TStack::push_back(p_data);

				m_oTime[0] += std::chrono::high_resolution_clock::now() - start;
				m_oCounter[0]++;

				return ret;
			}
			inline int push_back(TType&& p_data)
			{				
				auto start = std::chrono::high_resolution_clock::now();
				auto ret = TStack::push_back(p_data);

				m_oTime[1] += std::chrono::high_resolution_clock::now() - start;
				m_oCounter[1]++;

				return ret;

			}

			//Copys element to next free space
			inline int push_back(std::unique_ptr<const TType[]> p_data, const unsigned int p_size)
			{				
				auto start = std::chrono::high_resolution_clock::now();
				auto ret = TStack::push_back(std::move(p_data), p_size);

				m_oTime[2] += std::chrono::high_resolution_clock::now() - start;
				m_oCounter[2]++;

				return ret;
			}

			inline int push_back(const TType* const p_data, const unsigned int p_size)
			{				
				auto start = std::chrono::high_resolution_clock::now();
				auto ret = TStack::push_back(p_data, p_size);

				m_oTime[3] += std::chrono::high_resolution_clock::now() - start;
				m_oCounter[3]++;

				return ret;
			}		

			//Get last element without removing (no copy)
			inline TType back()
			{
				auto start = std::chrono::high_resolution_clock::now();
				auto tmp = TStack::back();

				m_oTime[4] += std::chrono::high_resolution_clock::now() - start;
				m_oCounter[4]++;
				
				return tmp;
			}

			//Get last element without removing (no copy)
			inline TType pop_back()
			{
				auto start = std::chrono::high_resolution_clock::now();
				auto tmp = TStack::pop_back();				

				m_oTime[5] += std::chrono::high_resolution_clock::now() - start;
				m_oCounter[5]++;

				return tmp;
			}


			inline void clear()
			{
				m_oTime[6] = std::chrono::high_resolution_clock::now() - m_oStackCreate;

				TFunc(m_oTime, m_oCounter);

				for (int i = 0; i < m_oCounter.size() - 1; i++)
				{
					m_oCounter[i] = 0;
				}
				TStack::clear();

			}

		};

	}
}

