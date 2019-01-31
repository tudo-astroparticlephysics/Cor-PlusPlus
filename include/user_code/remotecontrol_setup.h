/* setup.h
 * this file is part of RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#pragma once

#include <array>
#include <iostream>
#include <functional>
#include <map>

#define SEND_EVENT_HEADER 1
#define EVENT_HEADER_ID 0x03

#define SEND_EVENT_END 1
#define EVENT_END_ID 0x04

#define SEND_RUN_HEADER 1
#define RUN_HEADER_ID 0x05

#define SEND_RUN_END 1
#define RUN_END_ID 0x06

void remotecontrol_setup()
{
    std::cout << "Setup of RemoteControl!" << std::endl;
}


