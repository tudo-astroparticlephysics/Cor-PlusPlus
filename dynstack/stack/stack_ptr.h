/* stack_ptr.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once

#include <cstdio>
#include <type_traits>
#include <memory>

#include <iostream>

#include "dynstack/stack/stack.h"

namespace dynstack
{
	template<class TReturn, class TStorage>
	class Stack;

	/// Class to abstract bins from stack
	/**
	 *  This class is currently not used
	 *  This class provides an abstraction from the stack storage access.
	 */
	template<class TReturn, class TStorage = TReturn>
	class StackPtr
	{
		//static_assert(std::is_convertible<TStorage, TReturn>::value );

		friend Stack<TReturn, TStorage> ;

		template<class TR, class TS>
		friend class StackPtr;

	private:

		TStorage* const m_sStack;		// Constant pointer to memory region that can be modified

		bool m_bStatus;

		//		typedef std::function<void(*)(const TType* const)> customDelete ;

		typedef void (*customDelete)(TStorage* const);
		const customDelete m_fDel;

	protected:

		constexpr StackPtr(TStorage* const item, const customDelete d = nullptr)
				: m_sStack(item), m_bStatus(true), m_fDel(d)
		{
		}

	public:
		constexpr StackPtr()
				: m_sStack(nullptr), m_bStatus(false), m_fDel(nullptr)
		{
		}

		StackPtr(const StackPtr<TReturn, TStorage>& rhs)
				: m_sStack(rhs.m_sStack), m_bStatus(rhs.m_bStatus), m_fDel(rhs.m_fDel)
		{
			rhs.m_bStatus = false;
		}

		template<class TR>
		StackPtr(StackPtr<TR, TStorage> && rhs)
				: m_sStack(rhs.m_sStack), m_bStatus(rhs.m_bStatus), m_fDel(rhs.m_fDel)
		{
			rhs.m_bStatus = false;
			const_cast<TStorage* &>(rhs.m_sStack) = nullptr;
		}

		template<class TR>
		StackPtr(StackPtr<TR, TStorage> && rhs, const customDelete d)
				: m_sStack(rhs.m_sStack), m_bStatus(rhs.m_bStatus), m_fDel(d)
		{
			rhs.m_bStatus = false;
			const_cast<TStorage* &>(rhs.m_sStack) = nullptr;
		}

		~StackPtr()
		{
			if (m_fDel != nullptr && status())
				m_fDel(m_sStack);
		}		

		inline operator TReturn() const
		{
			return static_cast<TReturn>(*this->m_sStack);
		}

		inline TReturn& operator*()
		{
			return (TReturn&) (*m_sStack);
		}


		inline bool operator==(const TReturn& rhs) const
		{
			return rhs == (const TReturn) (*m_sStack);
		}

		template<class T>
		operator T() const = delete;

		inline TStorage* _get()
		{
			return m_sStack;
		}

		StackPtr<TReturn, TStorage>& operator=(const TStorage& rhs)
		{
			if (status())
				return StackPtr<TReturn, TStorage>();

			*(this->m_sStack) = rhs;
			return *this;
		}

		bool status() const
		{
			if (!m_bStatus || m_sStack == nullptr)
				return false;

			return true;
		}

		inline bool isNull() const
		{
			return m_sStack == nullptr;
		}

		void clear()
		{
			if (m_fDel != nullptr && status())
			{
				m_fDel(m_sStack);
			}

			m_bStatus = false;
			const_cast<TStorage* &>(m_sStack) = nullptr;
		}

	};

}



