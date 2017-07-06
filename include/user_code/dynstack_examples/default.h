/* default.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/
#pragma once

#include "basic/particle_deduction.h"

#include "dynstack/stack/storage/lifo_stack.h"

#include <memory>
#include <vector>
#include <list>
#include <string>


auto dynstack_setup(std::vector<long> sizes, std::vector< std::list<std::string> > arguments ) -> decltype(std::unique_ptr< dynstack::storage::LIFO_Stack<DeductedParticleType> >())
{
	if(sizes.size() != 1)
        {
                std::cerr << "You need to define the size of a single stack with DYNSTACK N in the input card" << std::endl;
		std::cerr << "You provided " << sizes.size() << " Arguments" << std::endl;
		
                exit(-1);
        }
	const int stackSize = sizes[0];

	//auto stack = std::make_unique< dynstack::storage::LIFO_Stack<ParticleType> >(stackSize);
	auto stack = std::unique_ptr< dynstack::storage::LIFO_Stack<DeductedParticleType> >(new dynstack::storage::LIFO_Stack<DeductedParticleType>(stackSize));

	std::cout << "(dyn) Create default stack" << std::endl;

	return std::move(stack);
}

inline void reset(){}

inline void close() {}




