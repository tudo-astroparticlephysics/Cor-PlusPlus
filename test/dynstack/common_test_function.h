/* common_test_function.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once


#include <array>

namespace test
{	
	namespace dynstack
	{

		/// Common test function for dynstack
		/**
		 *  Test function that checks most of the basic error possiblitys:
		 *    -# Content is equal to the truth
		 *    -# Back & pop on empty stack
		 *    -# Overfill stack
		 *    -# Access to overwriten element
		 *
		 *  To test borderline cases the stack should have a size of exactly 12!
		 *  Each object to write should be different
		 *
		 *  \tparam TStack Type of stack that should be tested
		 *  \tparam TType Type of content thats saved in the stack
		 *
		 *  \param testObject Ready to use stack that should be tested
		 *  \param data Values that should be stored to the stack. Value 0 and 1 should always work and dont get removed.
		 *  \param result Truth that is checked against the content in the stack in sequential order
		 */
		template<class TStack, class TType, size_t TSizeResult>
		bool testStack(TStack& testObject, std::array<TType, 12> data, std::array<TType, TSizeResult> result)
		{
			if (testObject.capacity() != 12)
			{
				std::cerr << "Error - Capacity is not 12 but " << testObject.capacity() << "\t(can be ignored for multi-stacks)" << std::endl;
			}


			/// Test single element insert
			testObject.push_back(data[0]);
			testObject.push_back(std::move(data[1]));

			if (testObject.size() != 2)
			{
				std::cerr << "Error - Size calculation is not correct or element [0],[1] from data are not stored !" << std::endl;				
			}

			/// Test multi element insert with pointer
			std::array<float, 5> arr1 = { { data[2], data[3], data[4], data[5], data[6] } };
			testObject.push_back(arr1.data(), 5);

			/// Test multi element insert with unique_ptr
			std::unique_ptr<float[]> arr2(new float[5]);
			for (int i = 0; i < 5; i++)
			{
				arr2[i] = data[7 + i];
			}
			testObject.push_back(std::move(arr2), 5);

			/// Test if the stack size is as expected
			if (testObject.size() != TSizeResult)
			{
				std::cerr << "Error - Stack size after all inserts is not equal to result size!" << std::endl;
				std::cerr << "Is: " << testObject.size() << "  Should: " << TSizeResult << std::endl;					

				return false;
			}


			int counter = 0;

			/// Check if stack is empty
			while (testObject.size() > 0)
			{
				/// Retrieve element from stack without deletion
				auto ele = testObject.back();

				/// Check if content matches whith the user defined truth
				if ((TType)ele != result[counter])
				{
					std::cerr << "Error - Stack content is not as expected! Is: " << (TType)ele << " Should: " << result[counter] << " on position " << counter << std::endl;
					return false;
				}
				counter++;
				
				testObject.pop();
			}

			/// Overflow test
			for (int i = 0; i < 20; i++)
			{
				testObject.push_back(data[0]);
			}	
			

			/// Empty Stack
			/// \TODO stack.clear()
			for (unsigned int i = 0; i < testObject.capacity() + 5; i++)
			{
				testObject.pop_back();				
			}

			/// Test back, pop and pop_back on an empty stack
			for (int i = 0; i < 20; i++)
			{
				if (testObject.back() != TType())
				{
					std::cerr << "Error - Stack is not empty or don't returns a default constructed object at call of back()" << std::endl;
					return false;
				}

				if (testObject.pop() != false)
				{
					std::cerr << "Error - Stack is not empty or don't returns false" << std::endl;
					return false;
				}

				auto tmp3 = testObject.pop_back();
				if (tmp3 != TType())
				{
					std::cerr << "Error - Stack is not empty or don't returns a default constructed object at call of pop_back(): " << tmp3 << "/" << TType() << std::endl;
					return false;
				}
			}
			


			/// Try to access "inavlid" memory region
			/**
			 *  This should not happen because back() returns a copy, the bigger problem is reserve() this can access the underlying storage directly
			 */
			testObject.push_back(data[1]);
			auto backElement = testObject.back();
			TType old = backElement;
			const bool ret = testObject.pop();

			testObject.push_back(data[0]);

			if (old != backElement && ret != false)
			{
				std::cerr << "pop_back returns: " << ret << std::endl;

				std::cerr << "Element vor Push: " << old << std::endl;

				std::cerr << "Element nach Push: " << (TType)backElement << std::endl;
				return false;
			}


			/// Empty Stack
			for (int i = 0; i < 20; i++)
			{
				testObject.pop();
			}



			testObject.push_back(result[0]);

			auto number1 = testObject.back();
			if (number1 != result[0])
			{
				std::cerr << "Back is not what it should be!" << std::endl;
				return false;
			}
			auto number2 = testObject.pop_back();
			if (number2 != result[0])
			{
				std::cerr << "pop_back is not what it should be!" << std::endl;
				return false;
			}


			return true;
		}


		
	}
}
