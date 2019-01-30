/* maincontrol_test.h
 * this file is part of Dynstack/RemoteControl for CORSIKA
 *
 * Copyright (C) <2019> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#pragma once

#include "zmq.hpp"

#include "remote_control/export.h"


#include <array>
#include <algorithm>
#include <numeric>

namespace test
{
	namespace remote_control
	{
        TEST(EXPORT, setup)
        {
            std::string str("RC_IP 127.0.0.1 5671");
            int len = str.length();

            ASSERT_NO_THROW({
                remotecontrol_init_(str.c_str(), &len);
            });
        }

        TEST(EXPORT, start)
        {

        }

        TEST(EXPORT, evth)
        {
            std::array<float, 273> arr;
            std::iota(arr.begin(), arr.end(), 273*0);

            ASSERT_NO_THROW( remotecontrol_push_runh_( arr.data() ) );
        }

        TEST(EXPORT, evte)
        {

            std::array<float, 273> arr;
            std::iota(arr.begin(), arr.end(), 273*1);

            ASSERT_NO_THROW( remotecontrol_push_runh_( arr.data() ) );
        }

        TEST(EXPORT, runh)
        {

            std::array<float, 273> arr;
            std::iota(arr.begin(), arr.end(), 273*2);

            ASSERT_NO_THROW( remotecontrol_push_runh_( arr.data() ) );
        }

        TEST(EXPORT, rune)
        {

            std::array<float, 273> arr;
            std::iota(arr.begin(), arr.end(), 273*3);

            ASSERT_NO_THROW( remotecontrol_push_runh_( arr.data() ) );
        }

        TEST(EXPORT, end)
        {

        }
    }
}