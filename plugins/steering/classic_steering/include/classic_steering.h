/*
 * classic_steering.h
 *
 *  Created on: Oct 17, 2017
 *      Author: dominik
 */

#pragma once

#include <string>
#include <map>
#include <list>

#include <fstream>
#include <iostream>

#include <algorithm>

#include "steering.h"


class ClassicSteering : public Steering
{
private:

	std::map<std::string, std::list<std::string> > m_data;


	void removeDuplicateSpaces(std::string& str)
	{
		auto spaces = [](char lhs, char rhs)->bool { return (lhs == rhs) && (lhs == ' '); };

		std::string::iterator new_end = std::unique(str.begin(), str.end(), spaces);
		str.erase(new_end, str.end());
	}

	std::list<std::string> splitSpaces( const std::string& str)
	{
		std::list<std::string> ret;

		std::stringstream ss(str);
		std::string item;

		while (std::getline(ss, item, ' ')) {
			ret.push_back( item );
		}

		return ret;

	}

public:

	ClassicSteering(std::string path)
	{
		std::ifstream file(path);
		if(!file)
		{
			std::cerr << "Steering file " << path << " could not be found!" << std::endl;
		}

		std::string buf;
		while( std::getline(file, buf, '\n') )
		{
			std::cout << "Line: " << buf << "||" << std::endl;

			this->removeDuplicateSpaces(buf);

			auto firstSpace = buf.find(' ');

			m_data[ buf.substr(0,firstSpace) ] = this->splitSpaces( buf.substr(firstSpace + 1) );
		}
	}


	~ClassicSteering()
	{

	}

	std::list< std::string > operator[](std::string key)
	{

		if( m_data.find(key) == m_data.end() )
		{
			return std::list<std::string>();
		}

		return m_data[key];
	}
};



