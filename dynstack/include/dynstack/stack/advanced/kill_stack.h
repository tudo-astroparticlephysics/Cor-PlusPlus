
#pragma once

#include "dynstack/stack/stack.h"

namespace dynstack { namespace advanced {

/** @brief Abort shower simulation if a condition is met */
template<class TStack, bool (*TFunc)()>
class KillStack : public TStack
{

	static_assert(std::is_base_of<_Stack, TStack>::value, "TStack must be a Stack!");

public:
	template<typename ... TArgs>
	KillStack(TArgs&&... args)
			: TStack(std::forward<TArgs>(args)...)
	{
	}

	KillStack(KillStack<TStack, TFunc> && rhs)
		: TStack((TStack)rhs)
	{
	}
	
	unsigned long size()
	{
		return TFunc() ? 0 : TStack::size();
	}
};

}}
