/* unit_test.cpp
* this file is part of Cor-PlusPlus for CORSIKA
*
* Copyright (C) <2017> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"

namespace test
{
    namespace dynrng
    {
        int main(int argc, char **argv)
        {
            ::testing::InitGoogleTest(&argc, argv);
            return RUN_ALL_TESTS();
        }

    }
}
