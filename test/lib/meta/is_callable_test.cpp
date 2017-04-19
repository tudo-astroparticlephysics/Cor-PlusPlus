/* list_test.cpp
 * this file is part of Dynstack/RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#include <memory>
#include <array>

#include <iostream>
#include <sstream>
#include <string>

#include <type_traits>

#include "lib/meta/is_callable.h"

namespace test
{
	namespace meta
	{
		
		bool a1(const float* const t)
		{
			(void)(t);;
			return true;
		}
		bool b1(const int* const t)
		{
			(void)(t);;
			return true;
		}

		struct a2
		{
			bool operator()(const float* const t) const
			{
				(void)(t);;
				return true;
			}
		};
		struct aa2
		{
			bool operator()(const float* const t)
			{
				(void)(t);;
				return true;
			}
		};
		struct b2
		{
			bool operator()(const int* const t) const
			{
				(void)(t);;
				return true;
			}
		};
		struct c2
		{
			bool print()
			{
				return true;
			}
		};
		
		bool test_isCallable()
		{
			auto a3 = [](const float* const t)->bool
			{	(void)(t);return true;};
			auto b3 = [](const int* const t)->bool
			{	(void)(t);return true;};

			std::cout << "a1: " << ::lib::meta::is_callable<decltype(&a1), bool, const float* const>::value << std::endl;
			std::cout << "b1: " << ::lib::meta::is_callable<decltype(&b1), bool, const float* const>::value << std::endl;
			std::cout << "a2: " << ::lib::meta::is_callable<a2, bool, const float* const>::value << std::endl;
			std::cout << "aa2: " << ::lib::meta::is_callable<aa2, bool, const float* const>::value << std::endl;
			std::cout << "b2: " << ::lib::meta::is_callable<b2, bool, const float* const>::value << std::endl;
			std::cout << "a3: " << ::lib::meta::is_callable<decltype(a3), bool, const float* const>::value << std::endl;
			std::cout << "b3: " << ::lib::meta::is_callable<decltype(b3), bool, const float* const>::value << std::endl;

			std::cout << "c1: " << ::lib::meta::is_callable<const int, bool, const float* const>::value << std::endl;
			std::cout << "c2: " << ::lib::meta::is_callable<c2, bool, const float* const>::value << std::endl;//, const bool(const float* const)

			return true;
		}

	}
}
