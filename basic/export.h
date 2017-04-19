/* export.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once

extern "C" void baack_init_();

extern "C" void baack_elcut_(const double* p_line, const int* p_size);

extern "C" void baack_pama_(const double* const p_particle_mass, const int* p_size);


/** \defgroup HeaderModifier Functions that allow the modification of the CORSIKA Header that are written to file
 *  @{
 */
extern "C" void baack_modify_evth_(float* p_evth, const int* p_size);

extern "C" void baack_modify_evte_(float* p_evte, const int* p_size);

extern "C" void baack_modify_runh_(float* p_runh, const int* p_size);

extern "C" void baack_modify_rune_(float* p_rune, const int* p_size);
/// @}



extern "C" void baack_customcall_(const int* id, const void* data, const int* length);
