/* basic.cpp
 * this file is part of Dynstack/RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#include "basic/basic.h"

#include <cstddef>
#include <stdexcept>

Basic::Basic()
{

}

Basic& Basic::getInstance()
{
	static Basic singelton;
	return singelton;
}

double Basic::particleRestMass(const int particleID) const
{
	if (particleID < 0 || size_t(particleID) >= m_particleMass.size())
		throw std::out_of_range("particle ID is out of range");
	return m_particleMass[particleID];

}


double Basic::getELCUT(Basic::energy_cut p_id) const
{
	return m_elcut[ static_cast<unsigned int>(p_id) ];
}
