/* protocol_test.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once


#include "remote_control/communication/packet.h"


namespace test
{

	namespace remote_control
	{


		TEST(Packet, Construction)
        {
			::remote_control::communication::Packet p1(0x01, 0);
			auto ptr1 = p1.toByte();
			ASSERT_EQ(p1.header(), 0x01);
			ASSERT_EQ(p1.size(), 8);

			ASSERT_EQ(reinterpret_cast<const int*>(ptr1)[0], 8);
			ASSERT_EQ(reinterpret_cast<const int*>(ptr1)[1], 0x01);



			// -----------------------------------

			::remote_control::communication::Packet p2(0x03, 0);
			p2.append(1337);

			auto ptr2 = p2.toByte();
			ASSERT_EQ(p2.header(), 0x03);
			ASSERT_EQ(p2.size(), 12);

			ASSERT_EQ(reinterpret_cast<const int*>(ptr2)[0], 12);
			ASSERT_EQ(reinterpret_cast<const int*>(ptr2)[1], 0x03);
			ASSERT_EQ(reinterpret_cast<const int*>(ptr2)[2], 1337);



			// -----------------------------------

			::remote_control::communication::Packet p3(0x05, 0);
			p3.append(1);
			p3.append(2);
			p3.append(3);
			p3.append(4);

			auto ptr3 = p3.toByte();
			ASSERT_EQ(p3.header(), 0x05);
			ASSERT_EQ(p3.size(), 24);

			ASSERT_EQ(reinterpret_cast<const int*>(ptr3)[0], 24);
			ASSERT_EQ(reinterpret_cast<const int*>(ptr3)[1], 0x05);
			ASSERT_EQ(reinterpret_cast<const int*>(ptr3)[5], 4);



			// -----------------------------------

			::remote_control::communication::Packet p4(0x07, 8);
			p4.append(1);

			auto ptr4 = p4.toByte();
			ASSERT_EQ(p4.header(), 0x07);
			ASSERT_EQ(p4.size(), 12);

			ASSERT_EQ(reinterpret_cast<const int*>(ptr4)[0], 12);
			ASSERT_EQ(reinterpret_cast<const int*>(ptr4)[1], 0x07);
			ASSERT_EQ(reinterpret_cast<const int*>(ptr4)[2], 1);


        }

	}
}
