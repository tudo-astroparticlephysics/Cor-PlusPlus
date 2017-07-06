/* storage_test.cpp
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#include "test/dynstack/storage_test.h"



#include <array>

#include "dynstack/stack/storage/lifo_stack.h"
#include "dynstack/stack/storage/lifo_std_stack.h"
#include "dynstack/stack/storage/fifo_stack.h"
#include "dynstack/stack/storage/null_stack.h"

#include "test/dynstack/common_test_function.h"

namespace test
{
	namespace dynstack
	{
		bool LIFO()
		{
			::dynstack::storage::LIFO_Stack<float> tmp1(12);
			bool ret = testStack<decltype(tmp1), float, 12>(tmp1, { { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 } }, { { 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 } });
			
			std::cout << "Basic Test: " << std::boolalpha << ret << std::endl;
			
			
			
			// Fill 20 Elements
			for(int i=0; i<20; i++)
				tmp1.push_back((const float)i);

			if(tmp1.size() != 12)
				std::cerr << "Stack has more/less elements then storage: " << tmp1.size() << std::endl;

			while(tmp1.size())
			{
				static float counter = 11; 
				auto ele = tmp1.back();

				if((float) ele != counter)
					std::cerr << "Wrong element in Stack after \"overflow\" is " << (float)ele << "| should " << counter << std::endl;

				counter--;

				//ele.clear();
				tmp1.pop();
			}

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


		bool LIFO_std()
		{
			::dynstack::storage::LIFO_std_Stack<float> tmp1(12);
			bool ret = testStack<decltype(tmp1), float, 12>(tmp1, { { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 } }, { { 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 } });

			std::cout << "Basic Test: " << std::boolalpha << ret << std::endl;



			// Fill 20 Elements
			for (int i = 0; i<20; i++)
				tmp1.push_back((const float)i);

			if (tmp1.size() != 12)
				std::cerr << "Stack has more/less elements then storage: " << tmp1.size() << std::endl;

			while (tmp1.size())
			{
				static float counter = 11;
				auto ele = tmp1.back();

				if ((float)ele != counter)
					std::cerr << "Wrong element in Stack after \"overflow\" is " << (float)ele << "| should " << counter << std::endl;

				counter--;

				//ele.clear();
				tmp1.pop();
			}

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

		bool FIFO()
		{
			::dynstack::storage::FIFO_Stack<float> tmp1(12);

			bool ret = testStack<decltype(tmp1), float, 12>(tmp1, { { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 } },{ { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 } });
			std::cout << "Basic Test: " << std::boolalpha << ret << std::endl;
			
			// Fill 20 Elements
			for (int i = 0; i<20; i++)
				tmp1.push_back((const float)i);

			if (tmp1.size() != 12)
				std::cerr << "Stack has more/less elements then storage: " << tmp1.size() << std::endl;

			while (tmp1.size())
			{
				static float counter = 0;
				auto ele = tmp1.back();

				if ((float)ele != counter)
					std::cerr << "Wrong element in Stack after \"overflow\" " << (float)ele << "|" << counter<< std::endl;

				counter++;

				//ele.clear();
				tmp1.pop();
			}

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


		bool Null()
		{
			return true;
		}


	
	}
}
