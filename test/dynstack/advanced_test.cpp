/* advanced_test.cpp
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


#include "test/dynstack/advanced_test.h"

#include "dynstack/stack/storage/lifo_stack.h"
#include "dynstack/stack/storage/fifo_stack.h"

#include "dynstack/stack/advanced/debug_stack.h"
#include "dynstack/stack/advanced/copy_stack.h"
#include "dynstack/stack/advanced/dynamic_sorted_stack.h"


#include "test/dynstack/common_test_function.h"








namespace test
{
	namespace dynstack
	{
		namespace advanced
		{
			

			bool Debug()
			{
				::dynstack::advanced::DebugStack<float, ::dynstack::storage::LIFO_Stack> tmp1(12);
				testStack<decltype(tmp1), float, 12>(tmp1, { { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 } }, { { 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 } });

				return true;
			}
			
			bool Copy()
			{
				::dynstack::advanced::CopyStack<::dynstack::storage::LIFO_Stack<float>, ::dynstack::storage::LIFO_Stack<float> > tmp1(std::make_tuple(12), std::make_tuple(12));
				testStack<decltype(tmp1), float, 12>(tmp1, { { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 } }, { { 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 } });

				return true;
			}
			
			unsigned int sort(const float* const data)
			{
				if (*data < 0.0)
					return 0;
				else if (*data > 9.0)
					return 9;

				return (int)(*data + 0.5);
			}
			bool DynamicSorted()
			{

				::dynstack::advanced::AdvancedDiscreteSortedStack<
				 float, sort,
				 ::dynstack::storage::LIFO_Stack<float>,
				  ::dynstack::storage::LIFO_Stack<float>,
				   ::dynstack::storage::LIFO_Stack<float>,
					::dynstack::storage::LIFO_Stack<float>,
					 ::dynstack::storage::LIFO_Stack<float>,
					  ::dynstack::storage::LIFO_Stack<float>,
					   ::dynstack::storage::LIFO_Stack<float>,
						::dynstack::storage::LIFO_Stack<float>,
						 ::dynstack::storage::LIFO_Stack<float>,
						  ::dynstack::storage::LIFO_Stack<float>> tmp1(12, 12, 12, 12, 12, 12, 12, 12, 12, 12);

				testStack<decltype(tmp1), float, 12>(tmp1, { { 12, 9, 3, 8, 7, 6, 2, 5, 4, 10, 11, 1 } }, { { 1, 2, 3, 4, 5, 6, 7, 8, 11, 10, 9, 12 } });



				return true;
			}


			bool Combinations()
			{

				return true;
			}
		}
	
	}
}
