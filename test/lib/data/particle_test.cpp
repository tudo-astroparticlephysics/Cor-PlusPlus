/* particle_test.cpp
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/



#include "lib/data/corsika/particle.h"


namespace test
{

	namespace lib
	{

		namespace data
		{
			bool test_particle()
			{
				auto part1 = ::lib::data::make_particle< ::lib::data::basic >();

				if (part1.PARTICLE_TYPE != 0)
				{
					std::cerr << "Particle type should be on position 0!" << std::endl;
					return false;
				}

				if (part1.size() != 13)
				{
					std::cerr << "Size of particle data 1 is not 13!" << std::endl;
					return false;
				}
				

				auto part2 = ::lib::data::make_particle< ::lib::data::basic , ::lib::data::custom, 3, ::lib::data::curved>();

				if (part2.CUSTOM_START != 13)
				{
					std::cerr << "Custom start should be on position 13!" << std::endl;
					return false;
				}

				if (part2.CUSTOM_END != 15)
				{
					std::cerr << "Custom end should be on position 15!" << std::endl;
					return false;
				}

				if(part2.APPARENT_HEIGHT != 16)
				{
					std::cerr << "APPARENT_HEIGHT should be on position 16!" << std::endl;
					return false;
				}

				if (part2.size() != 19)
				{
					std::cerr << "Size of particle data 2 is not 19!" << std::endl;
					return false;
				}


				part2[part2.CUSTOM_START] = 2.2;

				if (part2[part2.CUSTOM_START] != 2.2)
				{
					std::cerr << "Coult not write to particle" << std::endl;
					return false;
				}

				return true;
			}
		}
	}
}
