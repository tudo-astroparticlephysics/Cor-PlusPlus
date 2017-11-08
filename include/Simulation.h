/* Simulation.h
 * this file is part of CorPP for CORSIKA
 *
 * Copyright (C) <2017> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */


#pragma once


class Simulation
{
private:



protected:




public:

	/** Initialization of values and tables for the simulation
	 *
	 */
	Simulation();


	/** Cleanup code after the whole simulation is finished
	 *
	 */
	~Simulation();



	/** Loop for the complete simulation of an individual event
	 *
	 */
	void simulate_event();


};
