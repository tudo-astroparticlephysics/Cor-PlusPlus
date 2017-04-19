/* export.cpp
 * this file is part of Dynstack/RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#include "basic/export.h"

#include <iostream>

#include "header_manager.h"
#include "basic.h"


#include <user_code/basic_setup.h>

void baack_init_()
{
	basic_setup();
}

void baack_elcut_(const double* p_line, const int* p_size)
{
	if (*p_size != sizeof(double) * 4)
	{
		std::cerr << "baack_elcut: The low level energy cut parameter received from CORSIKA has the wrong length - is " << *p_size << " should " << 4 * 4 << std::endl;
	}

	for (int i = 0; i < 4; i++)
	{
		SBasic().m_elcut[i] = p_line[i];
	}
}

void baack_pama_(const double* const p_particle_mass, const int* p_size)
{
	if (*p_size != sizeof(double) * 6000)
	{
		std::cerr << "baack_pama: The particle mass data received from CORSIKA has the wrong length - is " << *p_size << " should " << sizeof(double) * 6000 << std::endl;
	}

	for (int i = 0; i < 6000; i++)
	{
		SBasic().m_particleMass[i] = p_particle_mass[i];
	}
}

void baack_modify_evth_(float* p_evth, const int* p_size)
{
	if (*p_size != 4 * 273)
	{
		std::cerr << "baack_basic (evth): The event header data received from CORSIKA has the wrong length - is " << *p_size << " should " << 4 * 273 << std::endl;
	}

	/// Raw pointer is moved into header
	lib::data::EventHeader header(p_evth);

	for (auto itr : SHeaderManager().m_oCallbackEVTH)
	{
		itr(header);
	}

}

void baack_modify_evte_(float* p_evte, const int* p_size)
{
	if (*p_size != 4 * 273)
	{
		std::cerr << "baack_basic (evte): The event end data received from CORSIKA has the wrong length - is " << *p_size << " should " << 4 * 273 << std::endl;
	}

	/// Raw pointer is moved into header
	lib::data::EventEnd header(p_evte);

	for (auto itr : SHeaderManager().m_oCallbackEVTE)
	{
		itr(header);
	}
}

void baack_modify_runh_(float* p_runh, const int* p_size)
{
	if (*p_size != 4 * 273)
	{
		std::cerr << "baack_basic (runh): The run header data received from CORSIKA has the wrong length - is " << *p_size << " should " << 4 * 273 << std::endl;
	}

	/// Raw pointer is moved into header
	lib::data::RunHeader header(p_runh);

	for (auto itr : SHeaderManager().m_oCallbackRUNH)
	{
		itr(header);
	}
}

void baack_modify_rune_(float* p_rune, const int* p_size)
{
	if (*p_size != 4 * 273)
	{
		std::cerr << "baack_basic (rune): The run end data received from CORSIKA has the wrong length - is " << *p_size << " should " << 4 * 273 << std::endl;
	}

	/// Raw pointer is moved into header
	lib::data::RunEnd header(p_rune);

	for (auto itr : SHeaderManager().m_oCallbackRUNE)
	{
		itr(header);
	}
}


void baack_customcall_(const int* id, const void* data, const int* length)
{
	(void(id));
	(void(data));
	(void(length));

}
