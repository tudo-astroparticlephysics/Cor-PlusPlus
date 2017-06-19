/* if_stack.h
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
	namespace advanced
	{		

		template<class TStackIf, class TStackElse, bool(*TFunc)(const typename TStackIf::m_ReturnType* const)>
		class IfStack : public Stack<typename TStackIf::m_ReturnType, typename TStackIf::m_StackType>
		{
			static_assert(std::is_base_of<_Stack, TStackIf>::value, "TStackIf must be a Stack!");
			static_assert(std::is_base_of<_Stack, TStackElse>::value, "TStackElse must be a Stack!");
			static_assert(std::is_same<typename TStackIf::m_ReturnType, typename TStackElse::m_ReturnType>::value, "TStackIf and TStackElse must store the same type");
			

		private:

			typedef typename TStackIf::m_ReturnType TType;
			typedef typename TStackIf::m_StackType TTypeStack;

			TStackIf m_oStack_if;
			TStackElse m_oStack_else;

		protected:

					
		public:

			
			template<class ... TArgs>
			IfStack(TArgs... args)
				: m_oStack_if( args... ), m_oStack_else( args... )
			{ }


			
			IfStack(IfStack<TStackIf, TStackElse, TFunc> && rhs)
				: m_oStack_if( std::move(rhs.m_oStack_if) ), m_oStack_else( std::move(rhs.m_oStack_else) )
			{ }

			~IfStack()
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

