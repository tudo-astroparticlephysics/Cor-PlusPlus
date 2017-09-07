/* dynamic_sorted_stack.h
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
	namespace advanced
	{

		/// Auxiliary classes for AdvancedSortedStack
		/**
		 * This namespace includes helper to execute default stack commands on a single object (at runtime!) in a std::tuple
		 */
		namespace aux
		{
			/// Visiter for tuple (general)
			/**
			 *  This visiter structure allows runtime access to tuple elements, here with focus on a std::tuple of (different) stacks.
			 *  It does not provide direct access to the elements in the std::tuple but to the most important stack functions.
			 *  For the access it creates multiple if else statments with template recursion that should be optimized from the compiler to a switch statement.
			 *
			 *  This general struct has a specialisation \ref visit_impl<0> to end the recursion.
			 *
			 *  \tparam I Number of elements in the tuple, in most cases std::tuple_size<T>::value.
			 */
			template<class TType, size_t I>
			struct visit_impl
			{
				template<class T, class TData>
				static int push_at(T& tup, size_t idx, TData data)
				{
					if (idx == I - 1)
						return std::get<I - 1>(tup).push_back(std::forward<TData>(data));
					else
						return visit_impl<TType, I - 1>::push_at(tup, idx, data);
				}

				template<class T>
				static TType back_at(T& tup, size_t idx)
				{
					if (idx == I - 1)
						return std::get<I - 1>(tup).back();
					else
						return visit_impl<TType, I - 1>::template back_at(tup, idx);
				}

				template<typename T>
				static TType pop_back_at(T& tup, size_t idx)
				{
					if (idx == I - 1)
						return std::move(std::get<I - 1>(tup).pop_back());
					else
						return std::move(visit_impl<TType, I - 1>::pop_back_at(tup, idx));
				}

				template<class T>
				static bool pop_at(T& tup, size_t idx)
				{
					if (idx == I - 1)
						return std::get<I - 1>(tup).pop();
					else
						return visit_impl<TType, I - 1>::pop_at(tup, idx);
				}

				template<typename T>
				static unsigned int capacity_at(T& tup, size_t idx)
				{
					if (idx == I - 1)
						return std::get<I - 1>(tup).capacity();
					else
						return visit_impl<TType, I - 1>::capacity_at(tup, idx);
				}

				template<typename T>
				static void clear_at(T& tup, size_t idx)
				{
					if (idx == I - 1)
						std::get<I - 1>(tup).clear();
					else
						visit_impl<TType, I - 1>::clear_at(tup, idx);
				}

				template<typename T>
				static Stack<TType>* get_at(T& tup, size_t idx)
				{
					if (idx == I - 1)
						return &std::get<I - 1>(tup);
					else
						return visit_impl<TType, I - 1>::get_at(tup, idx);

				}
			};

			/// Visiter for tuple (end)
			/**
			 *  This visiter structure allows runtime access to tuple elements.
			 *  It creates multiple if cases that should be optimized from the compiler to a switch statement
			 *
			 *  This is the specialisation for the end case 0
			 *
			 *  \tparam I Number of elements in the tuple, in most cases std::tuple_size<T>::value.
			 */
			template<class TType>
			struct visit_impl<TType, 0>
			{
				template<typename T, class TData>
				static int push_at(T& tup, size_t idx, TData data)
				{
					(void)(tup);
					(void)(idx);
					(void)(data);
					return -1;
					//static_assert(false,"This should not happen!");
				}

				template<typename T>
				static TType back_at(T& tup, size_t idx)
				{
					(void)(tup);
					(void)(idx);
					return TType();
					//static_assert(false,"This should not happen!");
				}

				template<typename T>
				static TType pop_back_at(T& tup, size_t idx)
				{
					(void)(tup);
					(void)(idx);
					return TType();
					//static_assert(false,"This should not happen!");
				}

				template<typename T>
				static bool pop_at(T& tup, size_t idx)
				{
					(void)(tup);
					(void)(idx);
					return false;
					//static_assert(false,"This should not happen!");
				}

				template<typename T>
				static unsigned int capacity_at(T& tup, size_t idx)
				{
					(void)(tup);
					(void)(idx);
					return 0;
					//static_assert(false,"This should not happen!");
				}

				template<typename T>
				static void clear_at(T& tup, size_t idx)
				{
					(void)(tup);
					(void)(idx);
					return;
					//static_assert(false,"This should not happen!");
				}

				template<typename T>
				static Stack<TType>* get_at(T& tup, size_t idx)
				{
					(void)(tup);
					(void)(idx);
					return nullptr;
				}
			};
		}



		/// Sorted Stack
		/**
		*   A sorted stack implementation that allowes the usage of different stacks in each bin.
		*
		*   \tparam TType Basic storage type of all stacks
		*   \tparam TFunc Sorting function that divides incoming data into bins
		*   \tparam TStack List of different Stacks, where every stack presents a single bin from 0 to sizeof...(TStack) - 1
		*/
		template<class TType, unsigned int (*TFunc)(const TType* const), class ... TStack>
		class AdvancedSortedStack: public Stack<TType>
		{
			//static_assert(std::is_base_of<_Stack, TStack>::value, "TStack must be a Stack!");
			static_assert(!std::is_pointer<TType>::value, "Template parameter can't be a pointer!");

		private:

			std::tuple< meta::tuple::tupleUnpack<TStack>...> m_oStacks;

			unsigned int m_uiBinSize[sizeof...(TStack)];

			unsigned int m_uiCap = 0;

		protected:

		public:

		/// Constructor
		/**
		*   Takes a variable list of constructor arguments to forward them to the different stacks.
		*   If one stack needs multiple parameter they must be stored inside a tuple of the needed size and type.
		*
		*   \tparam TArgs Type of arguments that must be forwarded to the storage stacks
		*/
		template<class ... TArgs>
		AdvancedSortedStack(TArgs&&... args)
		: m_oStacks(std::forward<TArgs>(args)...)
		{
			static_assert(sizeof...(TArgs) == sizeof...(TStack), "Not enough Arguments for every DiscreteSorted Bin!");

			for (unsigned int i = 0; i < sizeof...(TStack); i++)
			{
				m_uiBinSize[i] = 0;
			}

			//tuple fold Meta::Tuple::foldTuple::foldl();

			for (unsigned int i = 0; i < sizeof...(TStack); i++)
				m_uiCap += aux::visit_impl<TType, sizeof...(TStack)>::capacity_at(m_oStacks, i);
		}

		/// Copy constructor
		/**
		*
		*/
		AdvancedSortedStack(AdvancedSortedStack<TType, TFunc, TStack...> && rhs)
		: m_oStacks(std::move(rhs.m_oStacks))
		{

			rhs.m_iLastStack = -1;

			for (unsigned int i = 0; i < sizeof...(TStack); i++)
				m_uiBinSize[i] = rhs.m_uiBinSize[i];
		}

		/// Destructor
		/**
		*
		*/
		~AdvancedSortedStack()
		{
		}

		/// Copy save
		/**
		*   Classifies the variable and stores it into the specific stack
		*   \param data Reference to data that should be stores. The reference will get passed to the next stack level.
		*   \return forwards the return of the underlying stack classes
		*/
		inline int push_back(const TType& data)
		{
			const unsigned int bin = TFunc(&data);

			const int tmp = aux::visit_impl<TType, sizeof...(TStack)>::push_at(m_oStacks, bin, std::forward<const TType>(data));

			if (tmp == 0)
			{
				m_uiBinSize[bin]++;
			}

			return tmp;
		}

		/// Move save
		/**
		*   Classifies the variable and stores it into the specific stack
		*   \param data Data itself that should be stores. The object will get moved to the next stack level.
		*   \return forwards the return of the underlying stack classes
		*/
		inline int push_back(TType&& data)
		{
			const unsigned int bin = TFunc(&data);

			const int tmp = aux::visit_impl<TType, sizeof...(TStack)>::push_at(m_oStacks, bin, std::forward<const TType>(data));

			if (tmp == 0)
			{
				m_uiBinSize[bin]++;
			}

			return tmp;
		}

		/// Move array save
		/**
		*   Classifies each element and stores it into the specific stack
		*   \param data Unique pointer to an object array
		*	\param elem Number of objects that should be stored
		*   \return forwards the logical and of all returns from the underlying stack classes
		*/
		inline int push_back(std::unique_ptr<const TType[]> data, const unsigned int elem)
		{
			int check = 0;
			for (unsigned int i = 0; i < elem; i++)
			{
				const unsigned int bin = TFunc(&data[i]);

				const int tmp = aux::visit_impl<TType, sizeof...(TStack)>::push_at(m_oStacks, bin, data[i]);

				if (tmp == 0)
				{
					m_uiBinSize[bin]++;
				}

				check = check | tmp;
			}
			return check;

		}

		/// Array save
		/**
		*   Classifies each element and stores it into the specific stack
		*   \param data Pointer to an object array
		*	\param elem Number of objects that should be stored
		*   \return forwards the logical and of all returns from the underlying stack classes
		*/
		inline int push_back(const TType* const data, const unsigned int elem)
		{
			int check = 0;
			for (unsigned int i = 0; i < elem; i++)
			{
				const unsigned int bin = TFunc(data + i);

				const int tmp = aux::visit_impl<TType, sizeof...(TStack)>::push_at(m_oStacks, bin, data[i]);

				if (tmp == 0)
				{
					m_uiBinSize[bin]++;
				}

				check = check | tmp;
			}
			return check;
		}

		/// Copy highest priority element
		/**
		*   Returnes the next element from the first not empty stack, starting with stack 0.
		*   The element will be copied and doesn't get removed
		*   \return Copy of single stack element
		*/
		inline TType back()
		{
			for (unsigned int i = 0; i < sizeof...(TStack); i++)
			{
				if (m_uiBinSize[i] > 0)
				{
					return aux::visit_impl<TType, sizeof...(TStack)>::back_at(m_oStacks, i);
				}
			}
			return TType();
		}

		/// Move highest priority element
		/**
		*   Returnes the next element from the first not empty stack, starting with stack 0.
		*   The element will get removed.
		*   \return forwards the return from the underlying stack class
		*/
		inline TType pop_back()
		{
			for (unsigned int i = 0; i < sizeof...(TStack); i++)
			{
				if (m_uiBinSize[i] > 0)
				{
					m_uiBinSize[i]--;
					TType ret = aux::visit_impl<TType, sizeof...(TStack)>::pop_back_at(m_oStacks, i);
					return ret;
				}
			}
			return TType();
		}

		/// Remove last element
		/**
		*   Removes the last element that got called with back.
		*   \return forwards the return from the underlying stack class or false if back() was not called before.
		*/
		inline bool pop()
		{
            for (unsigned int i = 0; i < sizeof...(TStack); i++)
			{
				if (m_uiBinSize[i] > 0)
				{
					m_uiBinSize[i]--;
					return aux::visit_impl<TType, sizeof...(TStack)>::pop_at(m_oStacks, i);
				}
			}

			return false;
		}

		/// Clears the stack
		/**
		*   Calls clear on all substack. In normal cases this leads to complete cleanup of the every stack.
		*/
		inline void clear()
		{
			for (unsigned int i = 0; i < sizeof...(TStack); i++)
			{
				m_uiBinSize[i] = 0;

				aux::visit_impl<TType, sizeof...(TStack)>::clear_at(m_oStacks, i);
			}		
		}

		//Get new empty element to write into (direct access, zero copy)
		StackPtr<TType, TType> reserve()
		{
			return StackPtr<TType, TType>();
		}

		/// Stack size
		/**
		*   Get the added number of all elements stored in the different stacks
		*   \return Returnes the sum of each individual stack sizes
		*/
		inline unsigned long size() const
		{
			int sum = 0;
			for (unsigned int i = 0; i < sizeof...(TStack); i++)
			{
				sum += m_uiBinSize[i];
			}

			return sum;
		}

		/// Stack capacity
		/**
		*   Get the summarized capacity of all stacks combinded
		*   \return Returnes the sum of each individual stack capacity
		*/
		unsigned long capacity() const
		{
			return m_uiCap;
		}

	};
}
}
