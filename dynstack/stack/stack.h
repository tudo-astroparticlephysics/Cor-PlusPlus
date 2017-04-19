/* stack.h
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
#include <cstring>

#include "dynstack/stack/stack_ptr.h"


/// Main namespace for the dynstack package
namespace dynstack
{


	template<class TReturn, class TStorage>
	class StackPtr;

	class _Stack
	{
	};

	template<class TReturn, class TStorage = TReturn>
	class Stack: public _Stack
	{
	private:

		static_assert(std::is_default_constructible<TReturn>::value, "Stack type needs a default constructor!");
		static_assert(std::is_copy_constructible<TReturn>::value, "Stack type needs a copy constructor!");
		static_assert(std::is_move_constructible<TReturn>::value, "Stack type needs a copy constructor!");

	protected:

		Stack()
		{
		}

		Stack(const Stack& rhs) = delete;

		inline StackPtr<TReturn, TStorage> createStackObj(TReturn* const item, void (*del)(TStorage* const) = nullptr) const
		{
			return StackPtr<TReturn, TStorage>(item, del);
		}

	public:

		typedef TReturn m_ReturnType;
		typedef TStorage m_StackType;

		virtual ~Stack()
		{

		}

		//Copys element to next free space
		virtual inline int push_back(const TReturn& data) = 0;
		virtual inline int push_back(TReturn&& data) = 0;
		virtual inline int push_back(std::unique_ptr<const TReturn[]> data, const unsigned int elem) = 0;
		virtual inline int push_back(const TReturn* const data, const unsigned int elem) = 0;


		//Get last element without removing 
		virtual TReturn back() = 0;

		//Get last element with removing (move)
		virtual TReturn pop_back() = 0;

		/// Removes element from storage
		virtual bool pop() = 0;

		/// Clear and remove all elements from the stack
		virtual void clear() = 0;
		

		//Get new empty element to write into (direct access, zero copy)
		virtual StackPtr<TReturn, TStorage> reserve() = 0;

		//Returns the number of currently stored elements
		virtual unsigned long size() const = 0;

		virtual unsigned long capacity() const = 0;

	};
}


