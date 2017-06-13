/* wrapper_test.cpp
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#include "dynstack/stack/wrapper/filter_stack.h"
#include "dynstack/stack/wrapper/in_callback_stack.h"
#include "dynstack/stack/wrapper/in_modify_stack.h"

#include "dynstack/stack/wrapper/sorted_stack.h"

#include "dynstack/stack/storage/lifo_stack.h"
#include "dynstack/stack/advanced/debug_stack.h"

#include "lib/data/controlled_value.h"



#include "test/dynstack/common_test_function.h"

#include <memory>
#include <array>

namespace test
{
	namespace dynstack
	{

		namespace wrapper
		{
		

			int inDecide(const float* const data)
			{
				if (data == nullptr)
					return false;

				return (bool) (*data <= 8);
			}

			bool Filter()
			{
				::dynstack::wrapper::FilterStack<::dynstack::storage::LIFO_Stack<float>, inDecide> tmp1(12);
				testStack<decltype(tmp1), float, 4>(tmp1, { { 12, 11, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2 } }, { { 10, 9, 11, 12 } });

				tmp1.push_back(12);
				tmp1.push_back(12);
				float tmp = tmp1.pop_back();
				if (tmp != 12)
				{
					std::cerr << "Error - pop_back returns the wrong number (Test 1)" << std::endl;
					return false;
				}
				if (tmp1.pop_back() != 12)
				{
					std::cerr << "Error - pop_back returns the wrong number (Test 2)" << std::endl;
					return false;
				}

				return true;


				/*auto comp = [](const float* data) -> bool {
					if (data == nullptr)
						return false;
					return (bool)(*data > 8);
				};*/

			}
				

			void call(const float* const data)
			{
				if (data == nullptr) return;
				return;

				//std::cout << "Callback: " << *data << std::endl;
			}

			bool InputCallback()
			{
				::dynstack::wrapper::InputCallbackStack<::dynstack::storage::LIFO_Stack<float>, call> tmp1(12);
				testStack<decltype(tmp1), float, 12>(tmp1, { { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 } }, { { 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 } });

				tmp1.push_back(12);
				tmp1.push_back(12);
				float tmp = tmp1.pop_back();
				if (tmp != 12)
				{
					std::cerr << "Error - pop_back returns the wrong number (Test 1)" << std::endl;
					return false;
				}
				if (tmp1.pop_back() != 12)
				{
					std::cerr << "Error - pop_back returns the wrong number (Test 2)" << std::endl;
					return false;
				}

				return true;
			}
			
			float mod(const float* const data)
			{
				return *data + 4;
			}
			bool InputModify()
			{
				::dynstack::wrapper::InputModifyStack<::dynstack::storage::LIFO_Stack<float>, mod> tmp1(12);

				testStack<decltype(tmp1), float, 12>(tmp1, { { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 } }, { { 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5 } });

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
			bool Sorted()
			{
				::dynstack::wrapper::SortedStack<::dynstack::storage::LIFO_Stack<float>, 10, sort> tmp1(12, 12, 12, 12, 12, 12, 12, 12, 12, 12);

				testStack<decltype(tmp1), float, 12>(tmp1, { { 12, 9, 3, 8, 7, 6, 2, 5, 4, 10, 11, 1 } }, { { 1, 2, 3, 4, 5, 6, 7, 8, 11, 10, 9, 12 } });

				return true;
			}


			bool prio(const float* const data)
			{
				return (*data % 2 == 0)?true, false;
			}
			bool Priority()
			{
				::dynstack::wrapper::PriorityStack<::dynstack::storage::LIFO_Stack<float>, sort> tmp1(12);

				testStack<decltype(tmp1), float, 12>(tmp1, { { 1,2,3,4,5,6,7,8,9,10,11,12 } }, { { 12, 10, 8, 6, 4, 2, 11, 9, 7, 5, 3, 1 } });
			}


			bool Combinations()
			{
				std::cout << "Combination 1" << std::endl;

				::dynstack::wrapper::InputModifyStack<
						::dynstack::wrapper::InputCallbackStack<::dynstack::wrapper::FilterStack<::dynstack::storage::LIFO_Stack<float>, inDecide>, call>, mod> tmp1(12);

				testStack<decltype(tmp1), float, 8>(tmp1, { { 11, 12, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2 } }, { { 14, 13, 12, 11, 10, 9, 16, 15} });

				
				return true;

			}
		}

	}
}
