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

#include "advanced/combination.h"
#include "advanced/copy.h"
#include "advanced/debug.h"
#include "advanced/advanced_sorted.h"
#include "advanced/if.h"

#include "storage/fifo.h"
#include "storage/lifo_std.h"
#include "storage/lifo.h"
#include "storage/null.h"

#include "wrapper/combination.h"
#include "wrapper/filter.h"
#include "wrapper/input_callback.h"
#include "wrapper/input_modify.h"
#include "wrapper/output_callback.h"
#include "wrapper/output_modify.h"
#include "wrapper/priority.h"
#include "wrapper/sorted.h"


namespace test
{
    namespace dynstack
    {
        int main(int argc, char **argv)
        {
            ::testing::InitGoogleTest(&argc, argv);
            return RUN_ALL_TESTS();
        }

    }
}
