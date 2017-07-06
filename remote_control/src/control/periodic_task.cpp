/* periodic_task.cpp
 * this file is part of Dynstack/RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#include "remote_control/control/periodic_task.h"

namespace remote_control
{

	PeriodicTask::PeriodicTask(std::function<void(void)> p_f, const std::chrono::milliseconds p_interval)
			: m_func(p_f), m_callInterval(p_interval), m_timeOfLastCall( std::chrono::high_resolution_clock::now() )
	{
	}


	void PeriodicTask::call()
	{
		this->call(  std::chrono::high_resolution_clock::now() );
	}

	void PeriodicTask::call(std::chrono::high_resolution_clock::time_point p_currentTime)
	{
		auto elapsedTime = p_currentTime - m_timeOfLastCall;

		if(elapsedTime > m_callInterval)
		{
			m_timeOfLastCall = p_currentTime;
			m_func();
		}
		return;

	}
}

