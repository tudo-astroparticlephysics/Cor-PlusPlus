/* setup.h
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
typedef DeductedParticleType Particle;



#ifdef __DYNDEFAULT__
#include "user_code/dynstack_examples/default.h"
#elif __DYNOUTPUT__
#include "user_code/dynstack_examples/output.h"
#elif __DYNCUSTOM__
#include <vector>
#include <string>
#include <list>

/// Setup routine for the DynamicStack
/**
*   This function is called a single time on the initialization of CORSIKA. The stack and all needed global variables and functions should be setup here.
*   The arguments parameter can be used to set user defined threshold or variables used in the stack.
*
*   \warning Never return different stack types depending on inputcard arguments! The stack type must be known at compile time.
*   \param sizes Forwarded stack sizes from the <b>DYNSTACK N</b> steeringcard command. You can use multiple DYNSTACK N commands with a single argument each
*                                       or multiple "N" in a single DYNSTACK command. All will pushed into this vector and keep their order.
*   \param arguments Forwarded arguments from the  <b>DYNSTACK_P X</b> steeringcard command. Each DYNSTACK_P setting creates an own entry in the vector.
*                                               Each space separated word creats an entry in the list.
*/
auto dynstack_setup(std::vector<long> sizes, std::vector< std::list<std::string> > arguments )
{	
	// Remove error if all modifications are done
	#error Now stack defined
        return ;

        (void)(sizes);
        (void)(arguments);
}


/// Reset routine for the DynamicStack
/**
*   This function is called after every shower event and can be used to reset intermediate values calculated on one shower.
*/
void reset()
{

}

/// Closing routine for the DynamicStack
/**
*   This function is called at the shutdown of corsika at the end of the complete simulation run 
*/
void close()
{

}

#else
#error No stack defined
#endif


