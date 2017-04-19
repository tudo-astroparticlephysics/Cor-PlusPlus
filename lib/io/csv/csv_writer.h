/* csv_writer.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once


#include <tuple>
#include <fstream>


#include "lib/meta/index_sequence.h"

template<class ... TArgs>
class CSV_Writer
{
private:
	std::tuple<const TArgs&...> m_tup;

	const char m_cDelim;

	std::fstream m_oFile;

	template<size_t ... TIs>
	std::string elements_impl(lib::meta::integer_sequence<TIs...>)
	{
		std::stringstream sstr;
		const char delim = m_cDelim;

		auto concat_array = [&sstr, delim](const std::string* data, size_t size) -> void
		{
			for (int i = 0; i < size; i++)
			{
				sstr << data[i] << delim;
			}
		};

		// Expand all elements in tuple and call the lambda on every single one
		auto empty = { 0, (concat_array(std::get<TIs>(m_tup).getElements().data(), std::get<TIs>(m_tup).getElements().size()), 0)... };
		(void)(empty);

		return sstr.str();
	}

	template<size_t ... TIs>
	std::string names_impl(lib::meta::integer_sequence<TIs...>)
	{
		std::stringstream sstr;
		const char delim = m_cDelim;

		auto concat_array = [&sstr, delim](const std::string* data, size_t size) -> void
		{
			for (int i = 0; i < size; i++)
			{
				sstr << data[i] << delim;
			}
		};

		// Expand all elements in tuple and call the lambda on every single one
		auto empty = { 0, (concat_array(std::get<TIs>(m_tup).getNames().data(), std::get<TIs>(m_tup).getNames().size()), 0)... };
		(void)(empty);

		return sstr.str();
	}

public:
	CSV_Writer(const char* p_filename, const char p_delim, const TArgs& ... p_modules)
		: m_tup(p_modules...), m_cDelim(p_delim)
	{
		m_oFile.open(p_filename, std::ios::in | std::ios::out | std::ios::trunc);
		m_oFile.exceptions(std::ios::badbit | std::ios::failbit);

		m_oFile << this->names_impl(lib::meta::index_sequence_from< sizeof...(TArgs) >()) << '\n' << std::flush;
	}

	~CSV_Writer()
	{
		m_oFile.close();
	}


	void write()
	{
		std::string result = this->elements_impl( lib::meta::index_sequence_from< sizeof...(TArgs) >() );

		m_oFile << result << '\n' << std::flush;
	}



};
