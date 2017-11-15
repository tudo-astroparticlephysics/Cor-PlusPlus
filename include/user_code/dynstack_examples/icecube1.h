/* default.h
* this file is part of Dynstack/RemoteControl for ||IKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/
#pragma once

#include "basic/particle_deduction.h"

#include "basic/basic.h"

#include "dynstack/stack/storage/fifo_stack.h"
#include "dynstack/stack/storage/lifo_stack.h"
//#include "dynstack/stack/wrapper/priority_stack.h"
#include "dynstack/stack/advanced/if_stack.h"
#include "dynstack/stack/wrapper/filter_stack.h"
#include "dynstack/stack/wrapper/in_callback_stack.h"
#include "dynstack/stack/wrapper/out_callback_stack.h"


#include <memory>
#include <vector>
#include <list>
#include <string>



double energy_interesting; 	// ENERGY ABOVE WHICH PARTICLES ARE INTERESTING
int n_interesting;      	// NUMBER OF INTERESTING PARTICLES
int n_interesting_nu;   	// NUMBER OF INTERESTING NEUTRINOS
bool still_interesting; 	// FLAG INDICATING IF AN EVENT IS STILL INTERESTING


int first = 0;

//FSTACK (read from stack)
void callback_out(const DeductedParticleType* const p)
{
	if(first < 2){return;}
	//  IF THE PARTICLE COMING FROM THE STACK IS INTERESTING DECREASE THE
	//  COUNTER BY 1
	if( !(p->data()[0] == 1   ||  p->data()[0] == 2   ||
		p->data()[0] == 3   ||  p->data()[0] == 5   ||
		p->data()[0] == 6   ||  p->data()[0] == 7   ||
		p->data()[0] == 16  ||  p->data()[0] == 17  ||
		p->data()[0] == 18 )  &&
		p->data()[1]*SBasic().particleRestMass(p->data()[0]) >= energy_interesting )

	{
		n_interesting = n_interesting - 1;
	}

	// TSTEND (move from intermediate to final stack)
	still_interesting = n_interesting > 0;
}

int filter(const DeductedParticleType* const p)
{
	if(first < 2){return 0;}

	if(still_interesting)
	{
		return 0;	// Keep particle
	}
	return -1;
}

void callback_in(const DeductedParticleType* const p)
{ //To Stack (intermediate)

	first++;
	if(first < 2){return;}

	//  INCREASE NINTERSTING IF THE CURRENT SECPAR IS INTERESTING
	const unsigned int particle_type = p->data()[0];
	if( !( particle_type ==  1  ||  particle_type ==  2  ||
		 particle_type ==  3  ||  particle_type ==  5  ||
		 particle_type ==  6  ||  particle_type ==  7  ||
		 particle_type == 16  ||  particle_type == 17  ||
		 particle_type == 18 )  &&
		 p->data()[1]*SBasic().particleRestMass(particle_type) >= energy_interesting )
	{
		//  NEUTRINO PROGENITOR
		n_interesting = n_interesting + 1;
	}
	else if( particle_type >= 66  && particle_type <= 69 &&  p->data()[1] >= energy_interesting )
	{	//  NEUTRINO
		n_interesting    = n_interesting + 1;
		n_interesting_nu = n_interesting_nu + 1;
	}


}



bool sort_function(const DeductedParticleType* const p)
{	// TSTOUT Part

	if( n_interesting_nu > 0)
	{
		return true;
	}
	switch((int)p->data()[0])
	{
		case 1:		// Gamma
		case 2:		// e+
		case 3:		// e-
		case 5:		// µ+
		case 6:		// µ-
		case 7:		// pi_0
		case 16:	// K^o_s
		case 17:	// \eta
		case 18:	// \Lambda
		{
			double pama = SBasic().particleRestMass((int)p->data()[0]);
			if( p->data()[1]*pama >= energy_interesting)
			{
				return true;
			}
			break;
		}
	}
	return false;
}


typedef dynstack::wrapper::FilterStack<
		dynstack::wrapper::InCallbackStack<
			dynstack::wrapper::OutCallbackStack<
				dynstack::advanced::IfStack<
					dynstack::storage::FIFO_Stack<DeductedParticleType>,
					dynstack::storage::LIFO_Stack<DeductedParticleType>,
				        sort_function>,
			callback_out>,
		callback_in>,
	filter> stacktype;



auto dynstack_setup(std::vector<long> sizes, std::vector< std::list<std::string> > arguments ) -> decltype(std::unique_ptr< stacktype >())
{
	if(sizes.size() != 1)
        {
                std::cerr << "You need to define the size of a single stack with DYNSTACK N in the input card" << std::endl;
		std::cerr << "You provided " << sizes.size() << " Arguments" << std::endl;

                exit(-1);
        }
	const int stackSize = sizes[0];


	if(arguments.size() != 1)
	{
		std::cerr << "You need to define the energie limit in the input card with DYNSTACK_P" << std::endl;
		std::cerr << "You provided " << arguments.size() << " Arguments" << std::endl;
		exit(-1);
	}
	std::stringstream sstr;
	sstr << arguments[0].front();
	sstr >> energy_interesting;

	std::cout << "Energiy cut of icecube1: " << energy_interesting << std::endl;



	//auto stack = std::make_unique< dynstack::storage::LIFO_Stack<ParticleType> >(stackSize);
	auto stack = std::unique_ptr< stacktype >(new stacktype(stackSize));

	std::cout << "(dyn) Create default stack" << std::endl;

	return std::move(stack);
}

inline void reset()
{
	still_interesting = true;
	n_interesting      = 0;
	n_interesting_nu   = 0;

	first = 0;
}


inline void close() {}
