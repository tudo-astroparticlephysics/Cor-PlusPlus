/* showerview.h
 * this file is part of RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#pragma once

#include <string>

#include "lib/data/corsika/particle.h"

#include <memory>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <sstream>


#include "dynstack/stack/stack.h"
#include "dynstack/stack/storage/lifo_stack.h"
#include "dynstack/stack/wrapper/in_callback_stack.h"
#include "dynstack/stack/wrapper/out_callback_stack.h"


#include "basic/particle_deduction.h"


std::fstream file;
DeductedParticleType buffer;

void callback_in(const DeductedParticleType* const p)
{
        //std::stringstream sstr;

        static int counter = 0;
        std::cout << "Particle " << counter++ << " type: " << p->m_oData[DeductedParticleType::basic::PARTICLE_TYPE] << " height: " << p->m_oData[DeductedParticleType::H] << std::endl;

        //From:
        file.write(reinterpret_cast<const char*>(buffer.m_oData.data()), sizeof(double) * 17);
        //To:
        file.write(reinterpret_cast<const char*>(p->m_oData.data()), sizeof(double) * 17);

        //file << sstr.str();
}

void callback_out(const DeductedParticleType* const p)
{
        buffer = *p;
}


typedef  dynstack::wrapper::OutputCallbackStack<
  	       dynstack::wrapper::InputCallbackStack<
        		 dynstack::storage::LIFO_Stack<DeductedParticleType>,
			 callback_in>,
	       callback_out> stacktype;

auto dynstack_setup(std::vector<long> sizes, std::vector< std::list<std::string> > arguments ) -> decltype(std::unique_ptr< stacktype >())
{
	if(sizes.size() != 1)
	{
		std::cerr << "You need to define the size of a single stack with DYNSTACK N in the input card" << std::endl;
		exit(-1);
	}
	const int size = sizes[0];
		
	if(arguments.size() != 1)
	{
		std::cerr << "You need to define the base name of the output file with DYNSTACK_P in the input card" << std::endl;
		exit(-1);
	}
	const std::string output_file = arguments[0].front();


	file.open(output_file, std::ios::out | std::ios::binary | std::ios::trunc);
	if (!file.good())
	{
		std::cerr << "Showerview storage file could not be opened! " << std::endl;
		std::cout << " good()=" << file.good();
		std::cout << " eof()=" << file.eof();
		std::cout << " fail()=" << file.fail();
		std::cout << " bad()=" << file.bad();
		exit(-1);
	}

	std::cout << "(Dyn) Showerview file ready!" << std::endl;
	
	auto stack = std::unique_ptr< stacktype >(new stacktype(size));

	/*
	auto stack = std::make_unique<dynstack::wrapper::OutputCallbackStack<	//C++14
		dynstack::wrapper::InputCallbackStack<
		dynstack::storage::LIFO_Stack<DeductedParticleType>,
		callback_in>,
		callback_out>>(size);
	*/

	return std::move(stack);
}

inline void reset(){}

inline void close()
{
	file.close();
}
