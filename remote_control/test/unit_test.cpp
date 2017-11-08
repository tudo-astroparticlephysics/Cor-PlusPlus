/* unit_test.cpp
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/
#include "gtest/gtest.h"

#include "message_queue_test.h"
#include "packet_test.h"

namespace test
{
	namespace remote_control
	{
        int main(int argc, char **argv)
        {
            ::testing::InitGoogleTest(&argc, argv);
            return RUN_ALL_TESTS();
        }
    }
}
