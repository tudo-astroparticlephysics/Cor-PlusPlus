/* basic.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once

#include <array>


/// Class that gives access to all common data structures transfered from Fortran to c++
/**
 *  Singleton class that allows access to all basic information that CORSIKA release to the C++ core.
 *
 */
class Basic
{
private:
	std::array<double, 4> m_elcut;
	std::array<double, 6000> m_particleMass;

	Basic();

	friend void baack_elcut_(const double* p_line, const int* p_size);
	friend void baack_pama_(const double* const p_particle_mass, const int* p_size);


public:
	enum class energy_cut : unsigned int
		{
			hadron = 0,
			muon,
			electron,
			photon
		};


	static Basic& getInstance();


	double particleRestMass(const int particleID) const;
	double getELCUT(energy_cut p_type) const;

};


inline Basic& SBasic()
{
	return Basic::getInstance();
}

