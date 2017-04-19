/* file.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#pragma once


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


/// Small wrapper for file input and output
/**
*   Wrapper for fstream to write and read from files
*   Includes error checking and some convinient functions
*/
class File
{
private:
	std::fstream m_oFile;
	
	bool statusOpen = false;

public:
	
	void open(const char* p_name);
	
	void write(const char* const p_str);
	void write(const char* const p_str, const unsigned int p_len);
	void write(const std::string);
	
	/// Reads the complete file into a std::string
	/**
	 *
	 */
	const std::string read_complete();

	void flush();

	File(std::string p_fileName);
	File();
	
	~File();
	
	inline bool isOpen(){return this->statusOpen;}
};

