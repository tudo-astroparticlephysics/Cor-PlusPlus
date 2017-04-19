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

#include "lib/meta/compile_time_list.h"

using namespace lib::meta::list;

namespace test
{
	namespace meta
	{
		
		std::string concatStr(const int* a, std::string b)
		{
			std::stringstream sstr;
			sstr << *a;
			sstr << " ";
			sstr << b;
			return sstr.str();
		}

		std::string concatStr2(const int a, std::string b)
		{
			std::stringstream sstr;
			sstr << a;
			sstr << " ";
			sstr << b;
			return sstr.str();
		}

		int m(const int a)
		{
			return (a) + 5;
		}
		
		bool test_compileTimeList()
		{			
			static constexpr int t1 = 1, t2 = 2, t3 = 3, t4 = 4;
			constexpr int t5 = 5, t6 = 6, t7 = 7, t8 = 8, t9 = 9, t10 = 10;

			constexpr const int* p1 = &t1;
			constexpr const int* p2 = &t2;
			constexpr const int* p3 = &t3;
			constexpr const int* p4 = &t4;

			constexpr const List<const int, 4> tup1(t1, t2, t3, t4);
			constexpr const List<const int, 6> tup2(t5, t6, t7, t8, t9, t10);

			//constexpr List<const int*, 4> tupP3(p1, p2, p3, p4);
			constexpr auto tupP3 = make_list(p1, p2, p3, p4);


			std::cout << "Foldr: " << tup1.foldr(concatStr2, std::string(" ")) << std::endl;
			std::cout << "Foldl: " << tup1.foldl(concatStr2, std::string(" ")) << std::endl;
			std::cout << "Foldl: " << tupP3.foldl(concatStr, std::string(" ")) << std::endl;

			std::cout << "Map: " << (tup1.map<const int>(m)).foldr<std::string>(concatStr2, std::string(" ")) << std::endl;

			std::cout << "List concat: \n";
			//constexpr const auto tup3 = tup1.concat(tup2);
			std::cout << "\t" << tup1.concat(tup2).foldl(concatStr2, std::string(" ")) << std::endl;
			std::cout << "\t" << (tup1 + tup2).foldl(concatStr2, std::string(" ")) << std::endl;

			std::cout << "List sub: ";
			//constexpr const auto tup4 = tup2.subSection<3, 5>();
			std::cout << tup2.subSection<3, 5>().foldl(concatStr2, std::string(" ")) << std::endl;

			std::cout << "Get constexpr: " << tup1.get(0) << " " << tup1.get(1) << " " << tup1.get(2) << " " << tup1.get(3) << std::endl;
			std::cout << "Get template: " << tup1.get<0>() << " " << tup1.get<1>() << " " << tup1.get<2>() << " " << tup1.get<3>() << std::endl;

			std::cout << "Size: " << tup1.size() << std::endl;

			//static_assert( tup3.get(3) == &t3, "Test Assert"  );

			return true;
		}

	//*/bool TestList(){return true;}

	}
}
