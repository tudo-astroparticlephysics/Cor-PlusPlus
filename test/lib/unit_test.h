/*unit_test.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once

namespace test
{

	namespace lib
	{
		bool test_data();

		bool test_io();

		bool test_meta();
	}


	bool test_lib();
}

int main();

