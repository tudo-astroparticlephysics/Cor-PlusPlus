/* export.h
* this file is part of Cor-PlusPlus for CORSIKA
*
* Copyright (C) <2017> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once

extern "C" void dynrng_seeds(int* seeds);

extern "C" void dynrng_getrandom(int* gen, int* N, double* numbers);

