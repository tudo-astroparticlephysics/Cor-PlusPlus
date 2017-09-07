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

#include "test/dynstack/advanced/combination.h"
#include "test/dynstack/advanced/copy.h"
#include "test/dynstack/advanced/debug.h"
#include "test/dynstack/advanced/advanced_sorted.h"
#include "test/dynstack/advanced/if.h"

#include "test/dynstack/storage/fifo.h"
#include "test/dynstack/storage/lifo_std.h"
#include "test/dynstack/storage/lifo.h"
#include "test/dynstack/storage/null.h"

#include "test/dynstack/wrapper/combination.h"
#include "test/dynstack/wrapper/filter.h"
#include "test/dynstack/wrapper/input_callback.h"
#include "test/dynstack/wrapper/input_modify.h"
#include "test/dynstack/wrapper/output_callback.h"
#include "test/dynstack/wrapper/output_modify.h"
#include "test/dynstack/wrapper/priority.h"
#include "test/dynstack/wrapper/sorted.h"


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
