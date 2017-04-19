/* tuple_test.cpp
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
#include <tuple>

#include "lib/meta/tuple_algorithm.h"


using namespace lib::meta::tuple;


namespace test
{
	namespace meta
	{


		bool test_tupleFilter()
		{				
			auto a1 = std::make_tuple('a', "bc", 1, 'b', 2, 2.3f);
			std::cout << "A1: " << a1 << std::endl;

			std::cout << "is_same test: 0 - " << std::is_same<char, std::tuple_element<0, decltype(a1)>::type>::value << std::endl;
			std::cout << "is_same test: 1 - " << std::is_same<char, std::tuple_element<1, decltype(a1)>::type>::value << std::endl;
			std::cout << "is_same test: 2 - " << std::is_same<char, std::tuple_element<2, decltype(a1)>::type>::value << std::endl;
			std::cout << "is_same test: 3 - " << std::is_same<char, std::tuple_element<3, decltype(a1)>::type>::value << std::endl;
			std::cout << "is_same test: 4 - " << std::is_same<char, std::tuple_element<4, decltype(a1)>::type>::value << std::endl;
			std::cout << "is_same test: 5 - " << std::is_same<char, std::tuple_element<5, decltype(a1)>::type>::value << std::endl;



			auto b1 = lib::meta::tuple::filterTuple<char, decltype(a1) >::filter_same(a1);
			std::cout << "B1: " << b1 << std::endl;


			auto a2 = std::make_tuple();
			std::cout << "A2: " << a2 << std::endl;

			auto b2 = filterTuple<char, decltype(a2) >::filter_same(a2);
			std::cout << "B2: " << b2 << std::endl;

			return true;
		}




		const std::string fold(const char c, const std::string str)
		{
			//std::cout << "Str: " << str << "  " << " Char: " << c << std::endl;
			std::stringstream sstr;
			sstr << str;
			sstr << c;
			return sstr.str();
		}


		bool test_tupleFold()
		{
			auto a1 = std::make_tuple('a', 'b', 'c', 'd', 'e');

			std::cout << "Foldl: " << foldTuple::foldl<decltype(fold), std::string, decltype(a1)>(fold, a1, std::string("-")) << std::endl;
			std::cout << "Foldr: " << foldTuple::foldr<decltype(fold), std::string, decltype(a1)>(fold, a1, std::string("-")) << std::endl;


			auto a2 = std::make_tuple();
			std::cout << "Foldl: " << foldTuple::foldl<decltype(fold), std::string, decltype(a2)>(fold, a2, std::string("-")) << std::endl;
			std::cout << "Foldr: " << foldTuple::foldr<decltype(fold), std::string, decltype(a2)>(fold, a2, std::string("-")) << std::endl;


			return true;
		}


		int map(const int c)
		{
			return c - 3;
		}


		bool test_tupleMap()
		{			
			auto a1 = std::make_tuple(4, 5, 6, 7, 8);

			std::cout << "Map: " << mapTuple::map<decltype(map), decltype(a1)>(map, a1) << std::endl;


			return true;
		}

	}
}
