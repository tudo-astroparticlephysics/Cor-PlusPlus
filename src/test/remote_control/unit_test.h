/* unit_test.h
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

	namespace remote_control
	{				
		bool test_mesageQueue();

		bool test_protocol();

	}

	bool test_remoteControl();
}

int main();
