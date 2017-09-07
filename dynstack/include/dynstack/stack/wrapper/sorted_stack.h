/* sorted_stack.h
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

#include "meta/tuple/tuple_algorithm.h"

#include <type_traits>
#include <tuple>
#include <memory>
#include <array>


namespace dynstack
{
	namespace wrapper
	{

		template<class TStack, unsigned int TBins, unsigned int(*TFunc)(const typename TStack::m_ReturnType* const)>
		class SortedStack : public Stack<typename TStack::m_ReturnType, typename TStack::m_StackType>
		{
			static_assert(std::is_base_of<_Stack, TStack>::value, "TStack must be a Stack!");

		private:

			typedef typename TStack::m_ReturnType TType;
			typedef typename TStack::m_StackType TTypeStack;

			std::array<meta::tuple::tupleUnpack<TStack>, TBins> m_oStacks;   /// Array of stacks to store elements - tupleUnpack is only for construction of individual elements

			unsigned int m_uiBinSize[TBins]; /// Number of stored elements per bin (similar to bin.size())
			unsigned int m_uiSize;       /// Number of stored elements

			unsigned int m_uiCap = 0;    /// Capacity of all Stacks combined

		protected:


		public:


			template<class ... TArgs>
			SortedStack(TArgs&&... args)
				: m_oStacks{ { std::forward<TArgs>(args)... } }, m_uiSize(0)
			{
				static_assert(sizeof...(TArgs) == TBins, "Not enough Arguments for every DiscreteSorted Bin!");

				for (unsigned int i = 0; i < TBins; i++)
				{
					m_uiBinSize[i] = 0;
				}


				for (unsigned int i = 0; i < TBins; i++)// could be replaced with a fold function in c++17 to make it compiletime constant
                {
                    m_uiCap += m_oStacks[i].capacity();
                }
			}



			SortedStack(SortedStack<TStack, TBins, TFunc> && rhs)
				: m_oStacks( std::move(rhs.m_oStacks) ), m_uiSize(rhs.m_uiSize)
			{

				rhs.m_iLastStack = -1;
				rhs.m_uiSize = 0;

				for (unsigned int i = 0; i < TBins; i++)
					m_uiBinSize[i] = rhs.m_uiBinSize[i];
			}

			~SortedStack()
			{
			}

			inline int push_back(const TType& data)
			{
				const unsigned int bin = TFunc(&data);

				const int tmp = m_oStacks[bin].push_back(data);
				if (tmp == 0)
				{
					m_uiBinSize[bin]++;
					m_uiSize++;
				} // TODO Fallback solution with different stack (maybe additional wrapper?!)

				return tmp;
			}
			inline int push_back(TType&& data)
			{
				const unsigned int bin = TFunc(&data);

				const int tmp = m_oStacks[bin].push_back(data);
				if (tmp == 0)
				{
					m_uiBinSize[bin]++;
					m_uiSize++;
				}

				return tmp;
			}

			//Copys element to next free space
			inline int push_back(std::unique_ptr<const TType[]> data, const unsigned int elem)
			{
				int check = true;
				for (unsigned int i = 0; i < elem; i++)
				{
					const unsigned int bin = TFunc(&data[i]);

					const int tmp = m_oStacks[bin].push_back(data[i]);
					if (tmp == 0)
					{
						m_uiBinSize[bin]++;
						m_uiSize++;
					}

					check = check | tmp;
				}
				return check;

			}

			inline int push_back(const TType* const data, const unsigned int elem)
			{
				int check = true;
				for (unsigned int i = 0; i < elem; i++)
				{
					const unsigned int bin = TFunc(data + i);

					const int tmp = m_oStacks[bin].push_back(data[i]);
					if (tmp == 0)
					{
						m_uiBinSize[bin]++;
						m_uiSize++;
					}

					check = check | tmp;
				}
				return check;
			}

			//Get last element without removing (copy)
			inline TType back()
			{
				for (unsigned int i = 0; i < TBins; i++)
				{
					if (m_uiBinSize[i] > 0)
					{
						return m_oStacks[i].back();
					}
				}
				return TType();
			}

			//Get last element with removing (move)
			inline TType pop_back()
			{
				for (unsigned int i = 0; i < TBins; i++)
				{
					if (m_uiBinSize[i] > 0)
					{
						m_uiBinSize[i]--;
						m_uiSize--;
						return m_oStacks[i].pop_back();
					}
				}
				return TType();
			}


			// Removes last element from stack
			// that was readed with back
			inline bool pop()
			{
                for (unsigned int i = 0; i < TBins; i++)
				{
					if (m_uiBinSize[i] > 0)
					{
						m_uiBinSize[i]--;
						m_uiSize--;
						return m_oStacks[i].pop();
					}
				}

				return false;
			}

			inline void clear()
			{
				for (unsigned int i = 0; i < TBins; i++)
				{
					m_uiBinSize[i] = 0;
					m_oStacks[i].clear();
				}
				m_uiSize = 0;

			}

			//Get new empty element to write into (direct access, zero copy)
			StackPtr<TType, TTypeStack> reserve()
			{
				return StackPtr<TType, TTypeStack>();
			}



			inline unsigned long size() const
			{
				return m_uiSize;
			}

			unsigned long capacity() const
			{
				return m_uiCap;
			}


		};
	}
}
