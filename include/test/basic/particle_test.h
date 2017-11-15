/* particle_test.cpp
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/



#include "../basic/include/corsika/particle.h"


namespace test
{
	namespace basic
    {
			TEST(PARTICLE, Default)
			{
				auto part1 = ::lib::data::make_particle< ::lib::data::basic >();

				ASSERT_EQ(part1.PARTICLE_TYPE, 0) << "Particle type should be default 0";

                ASSERT_EQ(part1.size(), 13) << "Default particle has size 13";

            }

            TEST(PARTICLE, Custom)
            {
                auto part2 = ::lib::data::make_particle< ::lib::data::basic , ::lib::data::custom, 3, ::lib::data::curved>();

                ASSERT_EQ(part2.CUSTOM_START, 13);

                ASSERT_EQ(part2.CUSTOM_END, 15);

                ASSERT_EQ(part2.APPARENT_HEIGHT, 16);

			    ASSERT_EQ(part2.size(), 19)


                part2[part2.CUSTOM_START] = 2.2;
                ASSERT_EQ(part2[part2.CUSTOM_START], 2.2);
            }

		}
}
