/* priority_stack.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once

#include "dynstack/stack/stack.h"

#include <memory>
#include <array>


namespace dynstack
{
	namespace wrapper
	{		

		template<class TStack, bool(*TFunc)(const typename TStack::m_ReturnType* const)>
		class PriorityStack : public Stack<typename TStack::m_ReturnType, typename TStack::m_StackType>
		{
			static_assert(std::is_base_of<_Stack, TStack>::value, "TStack must be a Stack!");

		private:

			typedef typename TStack::m_ReturnType TType;
			typedef typename TStack::m_StackType TTypeStack;

			TStack m_oStack_if;
			TStack m_oStack_else;

		protected:

					
		public:

			
			template<class ... TArgs>
			PriorityStack(TArgs&&... args)
				: m_oStack_if(std::forward<TArgs>(args)... ), m_oStack_else(std::forward<TArgs>(args)...)
			{ }


			
			PriorityStack(PriorityStack<TStack, TFunc> && rhs)
				: m_oStack_if( std::move(rhs.m_oStack_if) ), m_oStack_else( std::move(rhs.m_oStack_else) )
			{ }

			~PriorityStack()
			{				
			}

			inline int push_back(const TType& data)
			{				
				const bool decision = TFunc(&data);				

				if (decision)
				{
					return m_oStack_if.push_back(data);
				}
				

				return m_oStack_else.push_back(data);
			}
			inline int push_back(TType&& data)
			{
				const bool decision = TFunc(&data);

				if (decision)
                                {
                                        return m_oStack_if.push_back(data);
                                }


                                return m_oStack_else.push_back(data);

			}

			//Copys element to next free space
			inline int push_back(std::unique_ptr<const TType[]> data, const unsigned int elem)
			{
				int check = true;
				for (unsigned int i = 0; i < elem; i++)
				{	
					const bool decision = TFunc(&data[i]);
					if (decision)
	                                {
                	                        check = check | m_oStack_if.push_back(data[i]);
        	                        }
					else
					{
        	                        	check = check | m_oStack_else.push_back(data[i]);
					}
					
				}
				return check;
				
			}

			inline int push_back(const TType* const data, const unsigned int elem)
			{
				int check = true;
				for (unsigned int i = 0; i < elem; i++)
                                {
                                        const bool decision = TFunc(data + i);
                                        if (decision)
                                        {
                                                check = check | m_oStack_if.push_back(data[i]);
                                        }
                                        else
                                        {
                                                check = check | m_oStack_else.push_back(data[i]);
                                        }

                                }
				return check;
			}

			//Get last element without removing (copy)
			inline TType back()
			{
				if (m_oStack_if.size() > 0)
				{
					return m_oStack_if.back();
				}
				else
				{
					return m_oStack_else.back();
				}
				return TType();
			}

			//Get last element with removing (move)
			inline TType pop_back()
			{
				if (m_oStack_if.size() > 0)
                                {
                                        return m_oStack_if.pop_back();
                                }
                                else
                                {
                                        return m_oStack_else.pop_back();
                                }
				return TType();
			}


			// Removes last element from stack 
			// that was readed with back
			inline bool pop()
			{
				if (m_oStack_if.size() > 0)
                                {
                                        return m_oStack_if.pop();
                                }
                                else
                                {
                                        return m_oStack_else.pop();
                                }

				return false;
			}

			inline void clear()
			{
				m_oStack_if.clear();
				m_oStack_else.clear();
			}

			//Get new empty element to write into (direct access, zero copy)
			StackPtr<TType, TTypeStack> reserve()
			{
				return StackPtr<TType, TTypeStack>();
			}

			

			inline unsigned long size() const
			{
				return m_oStack_if.size() + m_oStack_else.size();
			}

			unsigned long capacity() const
			{
				return m_oStack_if.capacity() + m_oStack_else.capacity();
			}


		};
	}
}

