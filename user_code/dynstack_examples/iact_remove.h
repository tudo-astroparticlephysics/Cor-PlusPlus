/* iact_remove.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/
#pragma once

#include "lib/data/corsika/particle.h"


#include "dynstack/stack/storage/lifo_stack.h"
#include "dynstack/stack/wrapper/filter_stack.h"


#include <memory>


#include "user_code/common/particle_deduction.h"
typedef DeductedParticleType Particle;


std::unique_ptr<dynstack::storage::LIFO_Stack<Particle>> create_iactremove(const unsigned int stackSize)
{
        auto stack = std::make_unique< dynstack::storage::LIFO_Stack<Particle> >(stackSize);

        std::cout << "(dyn) Create default stack" << std::endl;

        return std::move(stack);
}







