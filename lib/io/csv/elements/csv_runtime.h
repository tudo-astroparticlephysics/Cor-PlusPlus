/* csv_runtime.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*               All rights reserved.
*
*       This software may be modified and distributed under the terms
*       of the LGPL license. See the LICENSE file for details.
*/


#pragma once


#include <map>
#include <string>
#include <chrono>
#include <sstream>

#include <vector>


#include "lib/io/csv/csv_module.h"

template<int TNum>
class CSV_Runtime : public CSV_Module<TNum>
{
private:

	struct timer
	{
		std::chrono::duration<double, std::milli> cum_time;
		std::chrono::high_resolution_clock::time_point last_call;

		bool running;

		timer()
			:cum_time(std::chrono::duration<double, std::milli>(0.0)), last_call(std::chrono::high_resolution_clock::now()), running(false)
		{

		}
	};


	std::array<timer, TNum> m_oTime;

	std::map<std::string, unsigned int> m_oDict;

public:


	CSV_Runtime(std::array<std::string, TNum> p_timer)
	{
		for (int i = 0; i<TNum; i++)
		{
			std::cout << "Add " << p_timer[i] << " to csv list" << std::endl;
			m_oDict.insert(std::make_pair(p_timer[i], i));
		}
	}


	void start(const std::string& p_name)
	{
		this->start(m_oDict.at(p_name));
	}

	inline void start(unsigned int id)
	{
		timer& t = m_oTime[id];
		if (t.running == false)
		{
			t.running = true;
			t.last_call = std::chrono::high_resolution_clock::now();
		}

	}


	inline void stop(const std::string& p_name)
	{
		this->stop(m_oDict.at(p_name));
	}

	void stop(unsigned int id)
	{
		timer& t = m_oTime[id];
		if (t.running)
		{
			t.running = false;
			t.cum_time = std::chrono::high_resolution_clock::now() - t.last_call;
			t.last_call = std::chrono::high_resolution_clock::now();
		}
	}


	inline void toggle(const std::string& p_name)
	{
		this->toggle(m_oDict.at(p_name));
	}

	void toggle(unsigned int id)
	{
		timer& t = m_oTime[id];
		if (t.running)
		{
			t.running = false;
			t.cum_time = std::chrono::high_resolution_clock::now() - t.last_call;
			t.last_call = std::chrono::high_resolution_clock::now();
		}
		else
		{
			t.running = true;
			t.last_call = std::chrono::high_resolution_clock::now();
		}
	}




	void reset()
	{
		for (auto itr = m_oTime.begin(); itr != m_oTime.end(); itr++)
		{
			itr->cum_time = std::chrono::duration<double, std::milli>(0.0);
			itr->running = false;
		}
	}

	inline void reset(const std::string& p_name)
	{
		this->reset(m_oDict.at(p_name));
	}

	void reset(unsigned int id)
	{
		timer& t = m_oTime[id];

		t.cum_time = std::chrono::duration<double, std::milli>(0.0);
		t.running = false;
	}


	inline const timer& get(unsigned int id)
	{
		return m_oTime[id];
	}

	const timer& get(const std::string& p_name) const
	{
		this->get(m_oDict.at(p_name));
	}


	std::array<std::string, TNum> getNames() const
	{
		std::array<std::string, TNum> tmp;

		unsigned int counter = 0;
		for (auto itr : m_oDict)
		{
			tmp[itr.second] = itr.first;
			counter++;
		}

		return std::move(tmp);
	}

	std::array<std::string, TNum> getElements() const
	{
		std::array<std::string, TNum> tmp;

		for (int i = 0; i<TNum; i++)
		{
			std::stringstream sstr;

			sstr << m_oTime[i].cum_time.count();

			tmp[i] = sstr.str();
		}

		return tmp;
	}

};

