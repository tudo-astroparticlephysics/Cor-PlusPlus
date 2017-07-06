/* export.cpp
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/


#include <iostream>


#include <memory>
#include <map>
#include <string>
#include <list>
#include <vector>


#include "dynstack/export.h"

#include "user_code/dynstack_setup.h"



static std::list< std::pair< std::string, std::list<std::string>> > dynstack_steering_card;


/// Type of the stack defined in setuo
/**
 *  Gets the stacktype from the user defined setup routine and reduces it
 */
static decltype( dynstack_setup(std::vector<long>(), std::vector< std::list< std::string> >()) ) global_stack;


/**
 *
 *
 */
void dynstack_init_(const char* line, const int* size)
{
	std::stringstream sstr(std::string(line, *size));

	std::list<std::string> list;
	std::string item;
	while (std::getline(sstr, item, ' '))
	{
		if(item == " " || item == "")
		{
			continue;
		}
		else
		{
			list.push_back(item);
		}
	}

	item = list.front();
	list.pop_front();
	
	std::cout << item << " with " << list.size() << std::endl;

	dynstack_steering_card.push_back( std::pair<std::string, std::list<std::string>>(item, list) );
	


}


/**
*
*
*/
void dynstack_start_()
{
	if( dynstack_steering_card.size() == 0)
	{
		std::cerr << "Error: No stack size set!" << std::endl;
		std::cerr << "You need to set at least one size with \"DYNSTACK N\" in the steering card" << std::endl;
		exit(-1337);
	}

	std::vector<long> stack_size;
	std::vector<std::list<std::string>> additional_arguments;

	for (auto itr : dynstack_steering_card)
	{
		std::cout << "(Dyn) Setting: " << itr.first << " with ";
		for (auto tmp : itr.second)
		{
			std::cout << tmp << '\t';
		}
		std::cout << std::endl;

		if (itr.first == "DYNSTACK")
		{
			std::stringstream sstr;			

			for (auto tmp : itr.second)
			{
				long t;
				sstr << tmp;
				sstr >> t;
				stack_size.push_back(t);
			}					

		}
		else if (itr.first == "DYNSTACK_P")
		{
			additional_arguments.push_back(itr.second);
		}
		else
		{
			std::cerr << "(Dyn) Filter in corsika.f are not correct or unsupported setting" << std::endl;
			std::cerr << "(Dyn) " << itr.first << std::endl;
		}
	}

	global_stack = dynstack_setup(stack_size, additional_arguments);

	dynstack_reset_();
}

/**
*
*
*/
void dynstack_tstout_(const void* par, const int* size, int* ret)
{
	if (*size > static_cast<int>(sizeof(Particle)) )
	{
		std::cerr << "Size of particle is smaller then the requested storage from Corsika " << *size << "/" << sizeof(Particle);
		std::cin.get();
		exit(-1);
	}


	Particle p;
	std::memcpy(p.data(), par, *size);

	*ret = global_stack->push_back(p);	

	return;
}

/**
*
*
*/
void dynstack_fstack_(void* par, int* size)
{
	if (*size < static_cast<int>(sizeof(Particle)) )
	{
		std::cerr << "Size of particle is greater then the storage provided from Corsika " << *size << "/" << sizeof(Particle);
		std::cin.get();
		exit(-1);
	}


    auto p = global_stack->pop_back();
	std::memcpy(par, p.data(), *size);

	return;
}

/**
*
*
*/
void dynstack_empty_(bool* par)
{
    *par = (global_stack->size() == 0);

	return;
}

/**
*
*
*/
void dynstack_reset_()
{	
	///TODO: retrieve EVTE and push it for modifications to reset

	reset();

	global_stack->clear();	
}


void dynstack_end_()
{
	close();

	std::cout << "(Dyn) Terminate Stack" << std::endl;
}

