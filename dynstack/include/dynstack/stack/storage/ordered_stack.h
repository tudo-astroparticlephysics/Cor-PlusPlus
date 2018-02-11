/* ordered_stack.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2018> <Jakob van Santen>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once

#include <type_traits>

#include "dynstack/stack/stack.h"

#include <set>

namespace dynstack { namespace storage {

/** @brief Ordered storage
 *  Elements are popped in the order given by Compare.
 */
template <typename TType, typename Compare=std::less<TType>>
class OrderedStack : public Stack<TType>
{
	static_assert(!std::is_pointer<TType>::value, "Template parameter can't be a pointer!");

public:
	OrderedStack(size_t __attribute__((unused)) capacity=0) {}
	
	virtual int push_back(const TType &p)
	{
		storage_.insert(p);
		return 0;
	}

	virtual int push_back(TType &&p)
	{
		storage_.emplace(p);
		return 0;
	}

	virtual int push_back(std::unique_ptr<const TType[]> data, const unsigned int elem)
	{
		return push_back(data[elem]);
	}

	virtual int push_back(const TType* data, const unsigned int elem)
	{
		return push_back(data[elem]);
	}

	virtual TType back() {
		if (storage_.empty()) {
			return TType();
		} else {
			return *storage_.cbegin();
		}
	}

	virtual TType pop_back() {
	
		if (!storage_.empty()) {
			TType p = *storage_.cbegin();
			storage_.erase(storage_.cbegin());
		
			return p;
		}
		return TType();
	}

	virtual bool pop() {
		if (!storage_.empty()) {
			storage_.erase(storage_.cbegin());
			return true;
		}
		return false;
	}

	virtual unsigned long size() const { return storage_.size(); }
	virtual unsigned long capacity() const { return storage_.max_size(); }
	virtual void clear()
	{
		storage_.clear();
	}

private:
	std::set<TType, Compare> storage_;
};

}}
