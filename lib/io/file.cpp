/* file.cpp
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#include "lib/io/file.h"


#include <sstream>


File::File(std::string p_fileName) 
{

	m_oFile.open( p_fileName, std::ios::in | std::ios::out | std::ios::binary );

	if(m_oFile.bad())
	{
		statusOpen = false;
	}
	else
	{
		statusOpen = true;
	}
}

File::File() : statusOpen(false)
{

}

File::~File()
{
	m_oFile.close();
}

void File::open(const char* p_name)
{
	if(!statusOpen)
	{
		m_oFile.close();
	}

	m_oFile.open( p_name , std::ios::in | std::ios::out | std::ios::trunc | std::ios::binary);

	if(m_oFile.bad())
	{
		statusOpen = false;
		std::cerr << "ERROR, CAN\'T OPEN BAACK OUTPUT FILE" << std::endl;
	}
	else
	{
		statusOpen = true;
		m_oFile.flush();
	}

}

void File::write(const char* const p_str)
{
	int count=0;
	for(int i=0; i<4096; i++)
	{
		if(p_str[i] == '\0')
		{
			count = i;
			break;
		}
	}
	this->write(p_str, count);

}

void File::write(const char* const p_str, const unsigned int p_len)
{
	m_oFile.write(p_str, p_len);
}

void File::write(std::string p_str)
{
	std::cout << p_str << std::endl;
	for(int i=0; i<5; i++)
		std::cout << "-------------------" << std::endl;

	m_oFile.write(p_str.c_str(), p_str.length() );
}

const std::string File::read_complete()
{
	auto currentFilePos = this->m_oFile.tellg();
	this->m_oFile.seekg(0, std::ios::beg);


	std::stringstream buffer;
	buffer << this->m_oFile.rdbuf();

	this->m_oFile.seekg(currentFilePos, std::ios::beg);

	return buffer.str();
}



void File::flush()
{
	m_oFile.flush();
}
