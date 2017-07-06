/* particle_deduction.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/
#pragma once

#include "config.h"

#ifdef _NOCOCO_
#else
#include "../include/config.h"
#endif

#include "basic/corsika/particle.h"


using namespace data;


#if __MULTITHIN__
#define DEDUCTED_PARTICLE_COMPOSITION ParticleDataType::basic,\
										ParticleDataType::thin,\
										ParticleDataType::curved,\
										ParticleDataType::ehistory,\
										ParticleDataType::multithin_padding,\	// cell 39
										ParticleDataType::unused,\
										ParticleDataType::multithin
#else
	#if __PARALLEL__
	#define DEDUCTED_PARTICLE_COMPOSITION ParticleDataType::basic,\
											ParticleDataType::thin,\
											ParticleDataType::curved,\
											ParticleDataType::parallel1,			//Cell 18
											ParticleDataType::parallel_padding,\	//Cell 19-38
											ParticleDataType::parallel2				//Cell 39
	#else
		#if __EHISTORY__
			#define DEDUCTED_PARTICLE_COMPOSITION ParticleDataType::basic,\
													ParticleDataType::thin,\
													ParticleDataType::curved,\
													ParticleDataType::ehistory
		#else
			#if __INTTEST__
				#define DEDUCTED_PARTICLE_COMPOSITION ParticleDataType::basic,\
														ParticleDataType::thin,\
														ParticleDataType::curved,\
														ParticleDataType::inttest
			#else
				#if __CURVED__
					#define DEDUCTED_PARTICLE_COMPOSITION ParticleDataType::basic,\
															ParticleDataType::thin,\
															ParticleDataType::curved
				#else
					#if __THIN__
						#define DEDUCTED_PARTICLE_COMPOSITION ParticleDataType::basic,\
																ParticleDataType::thin
					#else
						#define DEDUCTED_PARTICLE_COMPOSITION ParticleDataType::basic
					#endif
				#endif
			#endif
		#endif
	#endif
#endif


/// Typedef to setup the particle type
/**
  *  This typedef defines the particle storage and its placement for the complete Dynstack implementation.
  *
  *  For a normal CORSIKA implementation the make_classic_particle function can be used.
  *  The template parameter should be the maximum number of the ParticleDataType that names a module you use in CORSIKA *
  *  As example CORSIKA is compiled with thinning, curved atmosphere and parallel -> parallel has a highest number in ParticleDataType of all three
  *  so ParticleDataType::parallel must be used to stay compatible with unmodified CORSIKA
  *
  *  It is also possible to use custom structures that describes the particle content. Its important to make sure that the correct padding is used and the binary structure
  *  matches with the one used in CORSIKA
  *
  *  Its additional possible to use make_particle to create a custom particle storage that can be compatible with unmodified CORSIKA. But allows more modifications
  *  to change the internal order of the storage for changes/optimization in the used CORSIKA particle format.
*/
typedef decltype(data::make_particle< DEDUCTED_PARTICLE_COMPOSITION >()) DeductedParticleType;


#if __cplusplus > 199711L
template<int TSize>
using DeductedParticleTypeCustom = decltype(data::make_particle< DEDUCTED_PARTICLE_COMPOSITION, ParticleDataType::custom, TSize>());
#else

template<int TSize>
struct DeductedParticleTypeCustom
{
	typedef decltype(data::make_particle< DEDUCTED_PARTICLE_COMPOSITION, ParticleDataType::custom, TSize>()) type;
};
#endif
