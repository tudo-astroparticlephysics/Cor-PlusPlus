/* unit_test.cpp
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/


#include "test/dynstack/storage_test.h"
#include "test/dynstack/wrapper_test.h"
#include "test/dynstack/advanced_test.h"


namespace test
{
	namespace dynstack
	{
		void test_storage()
		{
			std::cout << "\n\t-lifo-" << std::endl;
			test::dynstack::LIFO();
			std::cout << "\n\t-lifo std-" << std::endl;
			test::dynstack::LIFO_std();
			std::cout << "\n\t-fifo-" << std::endl;
			test::dynstack::FIFO();
			std::cout << "\n\t-null-" << std::endl;
			test::dynstack::Null();
		}

		void test_wrapper()
		{
			std::cout << "\n\t-filter-" << std::endl;
			test::dynstack::wrapper::Filter();

			std::cout << "\n\t-In Callback-" << std::endl;
			test::dynstack::wrapper::InputCallback();

			std::cout << "\n\t-In Mod-" << std::endl;
			test::dynstack::wrapper::InputModify();

			std::cout << "\n\t-Sorted-" << std::endl;
			test::dynstack::wrapper::Sorted();

			std::cout << "\n\t-Priority-" << std::endl;
			test::dynstack::wrapper::Priority();

			std::cout << "\n\t-Combination-" << std::endl;
			test::dynstack::wrapper::Combinations();
		}

		void test_advanced()
		{
			std::cout << "\n\t-Debug-" << std::endl;
			test::dynstack::advanced::Debug();			

			std::cout << "\n\t-Copy-" << std::endl;
			test::dynstack::advanced::Copy();

			std::cout << "\n\t-Dynamic Sorted-" << std::endl;
			test::dynstack::advanced::DynamicSorted();
		}

	}

	void test_dynstack()
	{
		std::cout << std::string(8, '\n');
		std::cout << "\t\t\t--- Dynstack Test ---" << std::endl;

		std::cout << "\n\n\t\t--Test storage--" << std::endl;
		dynstack::test_storage();

		std::cout << "\n\n\t\t--Test wrapper--" << std::endl;
		dynstack::test_wrapper();

		std::cout << "\n\n\t\t--Test advanced--" << std::endl;
		dynstack::test_advanced();

	}
}



int main()
{
	std::cout << "Start dynstack testing ... \n" << std::endl;
	
	test::test_dynstack();	

	

	std::cout << "\n\nAll tests finished " << std::endl;

	return 0;

}
