/* default.cpp
 * this file is part of RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */


#include <functional>
#include <iostream>

#include "user_code/remotecontrol_examples/default.h"



remote_control::PeriodicTask still_alive()
{
	std::function<void(void)> func = [](){
		std::cout << "B" << std::endl;
		return;
	};


	return remote_control::PeriodicTask(func, std::chrono::milliseconds(500) );
}
