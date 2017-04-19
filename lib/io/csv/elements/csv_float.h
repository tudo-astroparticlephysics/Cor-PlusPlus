/* csv_float.h
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
class CSV_Float : public CSV_Module<TNum>
{
private:

	std::array<double, TNum> m_oNumber;

	std::map<std::string, unsigned int> m_oDict;

public:


	CSV_Float(std::array<std::string, TNum> p_counter)
	{
		for (int i = 0; i<TNum; i++)
		{
			m_oDict.insert(std::make_pair(p_counter[i], i));
		}
	}


	void set(const std::string& p_name, double p_var)
	{
		this->m_oNumber[m_oDict.at(p_name)] = p_var;
	}
	

	void set(int p_id, double p_var)
	{
		this->m_oNumber[p_id] = p_var;
	}

	


	inline unsigned int get(unsigned int id) const
	{
		return m_oNumber[id];
	}

	inline unsigned int get(const std::string& p_name) const
	{
		this->get(m_oDict.at(p_name));
	}

	inline void setAll(double p_var)
	{
		for (int i = 0; i < TNum; i++)
		{
			m_oNumber[i] = p_var;
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

			sstr.precision(27);
			sstr << m_oNumber[i];

			tmp[i] = sstr.str();
		}
		return std::move(tmp);
	}

};

