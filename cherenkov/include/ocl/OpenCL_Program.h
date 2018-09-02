#pragma once


#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


#include "ocl/OpenCL_Include.h"


class OCL_Program
{
private:

	std::string m_path;


	std::string loadSource();
	
	cl::Program m_program;

public:

	OCL_Program(std::string path);


	cl::Program& load(cl::Device& device, cl::Context& context);
	

	inline cl::Program& getProgram() { return m_program; }



	void info();

};

