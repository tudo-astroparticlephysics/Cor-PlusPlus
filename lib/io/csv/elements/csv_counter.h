/* csv_counter.h
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
#include <sstream>

#include <vector>


#include "lib/io/csv/csv_module.h"

template<int TNum>
class CSV_Counter : public CSV_Module<TNum>
{
private:

	std::array<unsigned int, TNum> m_oCounter;

	std::map<std::string, unsigned int> m_oDict;

public:


	CSV_Counter(std::array<std::string, TNum> p_counter)
	{
		for (int i = 0; i<TNum; i++)
		{
			m_oDict.insert(std::make_pair(p_counter[i], i));
		}
	}


	void inc(const std::string& p_name)
	{
		this->m_oCounter[m_oDict.at(p_name)]++;
	}

	void dec(const std::string& p_name)
	{
		this->m_oCounter[m_oDict.at(p_name)]--;
	}

	void inc(int p_id)
	{
		this->m_oCounter[p_id]++;
	}

	void dec(int p_id)
	{
		this->m_oCounter[p_id]--;
	}


	inline unsigned int get(unsigned int id) const
	{
		return m_oCounter[id];
	}

	inline unsigned int get(const std::string& p_name) const
	{
		this->get(m_oDict.at(p_name));
	}

	inline void reset()
	{
		for (int i = 0; i < TNum; i++)
		{
			m_oCounter[i] = 0;
		}
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

			sstr << m_oCounter[i];

			tmp[i] = sstr.str();
		}
		return std::move(tmp);
	}

};

