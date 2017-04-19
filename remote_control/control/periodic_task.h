/* periodic_task.h
 * this file is part of Dynstack/RemoteControl for CORSIKA
 *
 * Copyright (C) <2016> <Dominik Baack>
 *		All rights reserved.
 *
 * 	This software may be modified and distributed under the terms
 * 	of the LGPL license. See the LICENSE file for details.
 */

#pragma once

#include <chrono>
#include <functional>


namespace remote_control
{
	

	class PeriodicTask
	{
	private:
		std::function<void(void)> m_func;
		const std::chrono::milliseconds m_callInterval;

		  std::chrono::high_resolution_clock::time_point m_timeOfLastCall;

	public:
		PeriodicTask(std::function<void(void)> p_f, const std::chrono::milliseconds p_interval);

		void call();
		void call(std::chrono::high_resolution_clock::time_point p_currentTime);

	};



}
