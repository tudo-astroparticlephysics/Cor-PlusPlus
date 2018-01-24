/* export.cpp
* this file is part of Cor-PlusPlus for CORSIKA
*
* Copyright (C) <2017> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/
#include "dynrng/export.h"

#include <array>
#include <random>


std::array<std::mt19937_64, 6> generators;


extern "C" void dynrng_seeds(double* seeds)
{
	// random device for true random
	for(int i=0; i<6; i++)
	{
		std::seed_seq seed(seeds[i]);

		generators[i].seed(seed);
	}
}

extern "C" void dynrng_getrandom(unsigned int* gen, int* N, double* numbers)
{
	std::uniform_real_distribution<double> distribution(0.0,1.0);

	if(*gen > 5)	// todo: change to debug mode only
	{
		return;
	}

	for(int i=0; i < *N; i++)
	{
		numbers[i] = distribution(generators[*gen]);
	}
}

