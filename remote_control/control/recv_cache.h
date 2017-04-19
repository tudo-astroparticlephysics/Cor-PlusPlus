/* recv_cache.h
 * this file is part of Dynstack/RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#pragma once

#include "lib/data/corsika/steering_card.h"


/// Cache information received from the central control instance
/**
 *  Caches steeringcard and next particle information to give immediate access in case CORSIKA requests them
 */
class recv_cache
{
private:

	lib::data::corsika::steering_card m_oSteering;




public:


};
